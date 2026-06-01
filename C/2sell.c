/*
 * 2sell.c
 * Sells 2 ADA (or any coin/amount) at market price on Bit24.
 * Edit BASE, QUOTE, and AMOUNT below to change coin or quantity.
 *
 * Dependencies: libcurl, openssl
 * Compile: gcc 2sell.c -o 2sell -lcurl -lssl -lcrypto
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>

/* ── Change these to sell a different pair or amount ── */
#define BASE   "ADA"
#define QUOTE  "IRT"
#define AMOUNT "2"
/* ───────────────────────────────────────────────────── */

#define BASE_URL "https://rest.bit24.cash"

/* ---------- dynamic response buffer ---------- */
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

int main(void)
{
    char api_key[256], secret_key[256];

    printf("Please Enter Your API Key    : ");
    fgets(api_key,    sizeof(api_key),    stdin);
    api_key[strcspn(api_key, "\n")] = '\0';

    printf("Please Enter Your Secret Key : ");
    fgets(secret_key, sizeof(secret_key), stdin);
    secret_key[strcspn(secret_key, "\n")] = '\0';

    /*
     * Sorted keys for signature:
     * amount, base_coin_symbol, category_type, quote_coin_symbol, type
     */
    char plain[512];
    snprintf(plain, sizeof(plain),
             "amount=%s&base_coin_symbol=%s&category_type=1"
             "&quote_coin_symbol=%s&type=0",
             AMOUNT, BASE, QUOTE);

    char sig[65];
    hmac_sha256_hex(secret_key, plain, sig);

    char body[600];
    snprintf(body, sizeof(body), "%s&signature=%s", plain, sig);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    if (!curl) { curl_global_cleanup(); return 1; }

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
    curl_global_cleanup();

    if (res != CURLE_OK) {
        fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));
        free(rb.data);
        return 1;
    }

    printf("Response: %s\n", rb.data ? rb.data : "(empty)");
    free(rb.data);
    return 0;
}
