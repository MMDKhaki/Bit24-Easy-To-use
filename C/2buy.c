/*
 * 2buy.c
 * Fetches best ask price for ADA/IRT and places a limit buy order on Bit24.
 * Change BASE, QUOTE, and AMOUNT below to buy a different coin/amount.
 *
 * Dependencies: libcurl, openssl
 * Compile: gcc 2buy.c -o 2buy -lcurl -lssl -lcrypto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

/* ── Change these to trade a different pair or amount ── */
#define BASE   "ADA"
#define QUOTE  "IRT"
#define AMOUNT "2"
/* ───────────────────────────────────────────────────── */

#define BASE_URL "https://rest.bit24.cash"
#define API_KEY_MAX    256
#define SECRET_KEY_MAX 256
#define BUF_SIZE       65536
#define PRICE_MAX      64

/* ---------- dynamic response buffer ---------- */
typedef struct {
    char  *data;
    size_t len;
} ResponseBuf;

static size_t write_cb(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    ResponseBuf *rb  = (ResponseBuf *)userdata;
    size_t       add = size * nmemb;
    rb->data = realloc(rb->data, rb->len + add + 1);
    if (!rb->data) return 0;
    memcpy(rb->data + rb->len, ptr, add);
    rb->len += add;
    rb->data[rb->len] = '\0';
    return add;
}

/* ---------- HMAC-SHA256 hex digest ---------- */
static void hmac_sha256_hex(const char *key, const char *msg, char *out_hex)
{
    unsigned char digest[32];
    unsigned int  dlen = sizeof(digest);
    HMAC(EVP_sha256(),
         key, (int)strlen(key),
         (const unsigned char *)msg, strlen(msg),
         digest, &dlen);
    for (int i = 0; i < 32; i++)
        sprintf(out_hex + i * 2, "%02x", digest[i]);
    out_hex[64] = '\0';
}

/* ---------- simple JSON field extractor ----------
   Finds  "key":"value"  or  "key":value  in json and copies the value.
   Returns 1 on success, 0 on failure.                                  */
static int json_get(const char *json, const char *key, char *out, size_t out_sz)
{
    char search[128];
    snprintf(search, sizeof(search), "\"%s\":", key);
    const char *p = strstr(json, search);
    if (!p) return 0;
    p += strlen(search);
    while (*p == ' ') p++;
    int quoted = (*p == '"');
    if (quoted) p++;
    size_t i = 0;
    while (*p && i < out_sz - 1) {
        if (quoted  && *p == '"')  break;
        if (!quoted && (*p == ',' || *p == '}' || *p == ' ')) break;
        out[i++] = *p++;
    }
    out[i] = '\0';
    return i > 0;
}

/* ---------- get best ask price ---------- */
static int get_best_ask(const char *api_key, char *price_out)
{
    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    char url[512];
    snprintf(url, sizeof(url),
             BASE_URL "/pro/capi/v1/markets/order-books"
             "?base_coin=%s&quote_coin=%s", BASE, QUOTE);

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

    if (res != CURLE_OK || !rb.data) { free(rb.data); return 0; }

    /* find first "price" inside sell_orders */
    const char *sell = strstr(rb.data, "sell_orders");
    int ok = 0;
    if (sell) {
        ok = json_get(sell, "price", price_out, PRICE_MAX);
    }
    free(rb.data);
    return ok;
}

/* ---------- submit limit buy order ---------- */
static int submit_limit_buy(const char *api_key, const char *secret_key,
                             const char *price)
{
    /*
     * Parameters must be sorted alphabetically for the signature.
     * Sorted keys: amount, base_coin_symbol, category_type, price,
     *              quote_coin_symbol, type
     */
    char plain[512];
    snprintf(plain, sizeof(plain),
             "amount=%s&base_coin_symbol=%s&category_type=0"
             "&price=%s&quote_coin_symbol=%s&type=1",
             AMOUNT, BASE, price, QUOTE);

    char sig[65];
    hmac_sha256_hex(secret_key, plain, sig);

    /* Build POST body (signature appended last – not part of signed string) */
    char body[700];
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

    printf("Response: %s\n", rb.data);

    char success[8], order_id[32], status[64], each_price[32], amount[32], total[32];
    json_get(rb.data, "success", success, sizeof(success));

    if (strcmp(success, "true") == 0) {
        json_get(rb.data, "id",          order_id,   sizeof(order_id));
        json_get(rb.data, "status_text", status,     sizeof(status));
        json_get(rb.data, "each_price",  each_price, sizeof(each_price));
        json_get(rb.data, "amount",      amount,     sizeof(amount));
        json_get(rb.data, "total",       total,      sizeof(total));
        printf("\nOrder placed successfully!\n");
        printf("Order ID : %s\n", order_id);
        printf("Status   : %s\n", status);
        printf("Price    : %s IRT\n", each_price);
        printf("Amount   : %s %s\n",  amount, BASE);
        printf("Total    : %s IRT\n", total);
    } else {
        char err[256] = "";
        json_get(rb.data, "error", err, sizeof(err));
        printf("Order failed: %s\n", err);
    }

    free(rb.data);
    return 1;
}

/* ---------- main ---------- */
int main(void)
{
    char api_key[API_KEY_MAX], secret_key[SECRET_KEY_MAX];

    printf("Please Enter Your API Key    : ");
    if (!fgets(api_key, sizeof(api_key), stdin)) return 1;
    api_key[strcspn(api_key, "\n")] = '\0';

    printf("Please Enter Your Secret Key : ");
    if (!fgets(secret_key, sizeof(secret_key), stdin)) return 1;
    secret_key[strcspn(secret_key, "\n")] = '\0';

    curl_global_init(CURL_GLOBAL_DEFAULT);

    char price[PRICE_MAX] = "";
    printf("Fetching best ask for %s/%s ...\n", BASE, QUOTE);
    if (!get_best_ask(api_key, price)) {
        fprintf(stderr, "Failed to fetch order book.\n");
        curl_global_cleanup();
        return 1;
    }
    printf("Best ask price (raw): %s %s\n", price, QUOTE);

    printf("Placing limit buy order for %s %s at %s %s ...\n",
           AMOUNT, BASE, price, QUOTE);
    submit_limit_buy(api_key, secret_key, price);

    curl_global_cleanup();
    return 0;
}
