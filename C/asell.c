/*
 * asell.c
 * Fetches available ADA balance from Bit24 wallet and sells it all at market.
 * Change COIN_SYMBOL and QUOTE below for a different pair.
 *
 * Dependencies: libcurl, openssl
 * Compile: gcc asell.c -o asell -lcurl -lssl -lcrypto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

/* ── Change these for a different coin/quote ── */
#define COIN_SYMBOL "ADA"
#define QUOTE       "IRT"
/* ─────────────────────────────────────────────── */

#define BASE_URL "https://rest.bit24.cash"

/* ---------- dynamic buffer ---------- */
typedef struct { char *data; size_t len; } ResponseBuf;

static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *ud)
{
    ResponseBuf *rb = (ResponseBuf *)ud;
    size_t add = size * nmemb;
    rb->data = realloc(rb->data, rb->len + add + 1);
    if (!rb->data) return 0;
    memcpy(rb->data + rb->len, ptr, add);
    rb->len += add;
    rb->data[rb->len] = '\0';
    return add;
}

/* ---------- HMAC-SHA256 hex ---------- */
static void hmac_sha256_hex(const char *key, const char *msg, char *out)
{
    unsigned char digest[32];
    unsigned int  dlen = sizeof(digest);
    HMAC(EVP_sha256(),
         key, (int)strlen(key),
         (const unsigned char *)msg, strlen(msg),
         digest, &dlen);
    for (int i = 0; i < 32; i++)
        sprintf(out + i * 2, "%02x", digest[i]);
    out[64] = '\0';
}

/* ---------- simple JSON value extractor ---------- */
static int json_get(const char *json, const char *key, char *out, size_t sz)
{
    char pat[128];
    snprintf(pat, sizeof(pat), "\"%s\":", key);
    const char *p = strstr(json, pat);
    if (!p) return 0;
    p += strlen(pat);
    while (*p == ' ') p++;
    int q = (*p == '"');
    if (q) p++;
    size_t i = 0;
    while (*p && i < sz - 1) {
        if ( q && *p == '"')  break;
        if (!q && (*p == ',' || *p == '}' || *p == ' ')) break;
        out[i++] = *p++;
    }
    out[i] = '\0';
    return i > 0;
}

/* ---------- fetch available balance for COIN_SYMBOL ---------- */
static double get_available_balance(const char *api_key)
{
    CURL *curl = curl_easy_init();
    if (!curl) return -1.0;

    const char *url = BASE_URL
        "/asset/capi/v1/wallet/assets"
        "?without_irt=0&without_zero=1";

    ResponseBuf rb = { malloc(1), 0 };

    struct curl_slist *hdrs = NULL;
    char hdr[300];
    snprintf(hdr, sizeof(hdr), "X-BIT24-APIKEY: %s", api_key);
    hdrs = curl_slist_append(hdrs, "Accept: application/json");
    hdrs = curl_slist_append(hdrs, hdr);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rb);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(hdrs);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK || !rb.data) { free(rb.data); return -1.0; }

    /*
     * Walk through "asset" array entries looking for COIN_SYMBOL.
     * Each entry looks like: {...,"symbol":"ADA","available_balance":"12.34",...}
     */
    double balance = 0.0;
    const char *p = rb.data;
    while ((p = strstr(p, "\"symbol\":")) != NULL) {
        char sym[32] = "";
        json_get(p, "symbol", sym, sizeof(sym));
        if (strcmp(sym, COIN_SYMBOL) == 0) {
            char bal[64] = "";
            json_get(p, "available_balance", bal, sizeof(bal));
            balance = atof(bal);
            break;
        }
        p++;
    }

    free(rb.data);
    return balance;
}

/* ---------- market sell all ---------- */
static int market_sell(const char *api_key, const char *secret_key,
                       double amount)
{
    char amount_str[64];
    snprintf(amount_str, sizeof(amount_str), "%g", amount);

    /*
     * Sorted keys:
     * amount, base_coin_symbol, category_type, quote_coin_symbol, type
     */
    char plain[512];
    snprintf(plain, sizeof(plain),
             "amount=%s&base_coin_symbol=%s&category_type=1"
             "&quote_coin_symbol=%s&type=0",
             amount_str, COIN_SYMBOL, QUOTE);

    char sig[65];
    hmac_sha256_hex(secret_key, plain, sig);

    char body[600];
    snprintf(body, sizeof(body), "%s&signature=%s", plain, sig);

    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    ResponseBuf rb = { malloc(1), 0 };

    struct curl_slist *hdrs = NULL;
    char hdr[300];
    snprintf(hdr, sizeof(hdr), "X-BIT24-APIKEY: %s", api_key);
    hdrs = curl_slist_append(hdrs, "Accept: application/json");
    hdrs = curl_slist_append(hdrs, "Content-Type: application/x-www-form-urlencoded");
    hdrs = curl_slist_append(hdrs, hdr);

    curl_easy_setopt(curl, CURLOPT_URL,
                     BASE_URL "/pro/capi/v1/orders/submit");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rb);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(hdrs);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK || !rb.data) { free(rb.data); return 0; }

    printf("Sell response: %s\n", rb.data);

    char success[8] = "";
    json_get(rb.data, "success", success, sizeof(success));
    if (strcmp(success, "true") == 0) {
        char order_id[32] = "";
        json_get(rb.data, "id", order_id, sizeof(order_id));
        printf("Order placed successfully! Order ID: %s\n", order_id);
    } else {
        char err[256] = "";
        json_get(rb.data, "error", err, sizeof(err));
        printf("Order error: %s\n", err);
    }

    free(rb.data);
    return 1;
}

/* ---------- main ---------- */
int main(void)
{
    char api_key[256], secret_key[256];

    printf("Please Enter Your API Key    : ");
    fgets(api_key,    sizeof(api_key),    stdin);
    api_key[strcspn(api_key, "\n")] = '\0';

    printf("Please Enter Your Secret Key : ");
    fgets(secret_key, sizeof(secret_key), stdin);
    secret_key[strcspn(secret_key, "\n")] = '\0';

    curl_global_init(CURL_GLOBAL_DEFAULT);

    printf("Fetching %s wallet balance ...\n", COIN_SYMBOL);
    double balance = get_available_balance(api_key);
    if (balance < 0) {
        fprintf(stderr, "Failed to fetch wallet.\n");
        curl_global_cleanup();
        return 1;
    }
    printf("%s available balance: %g\n", COIN_SYMBOL, balance);

    if (balance <= 0) {
        printf("No %s available to sell.\n", COIN_SYMBOL);
        curl_global_cleanup();
        return 0;
    }

    market_sell(api_key, secret_key, balance);

    curl_global_cleanup();
    return 0;
}
