# آموزش کامل سفارش‌گذاری در API صرافی Bit24 با زبان C

## ساختار کلی سفارش در Bit24

تمام سفارش‌ها در Bit24 با ارسال پارامترها به Endpoint زیر انجام می‌شوند:

```c
https://rest.bit24.cash/pro/capi/v1/orders/submit
```

نمونه پارامترهای یک سفارش:

```c
base_coin_symbol=ADA
quote_coin_symbol=IRT
type=0
category_type=1
amount=2
```

---

# توضیح کامل پارامترها

## `base_coin_symbol`

```c
#define BASE "ADA"
```

ارز اصلی‌ای که قصد خرید یا فروش آن را دارید.

مثال‌ها:

| ارز     | مقدار   |
| ------- | ------- |
| بیتکوین | `"BTC"` |
| اتریوم  | `"ETH"` |
| کاردانو | `"ADA"` |
| سولانا  | `"SOL"` |

---

## `quote_coin_symbol`

```c
#define QUOTE "IRT"
```

ارزی که معامله بر اساس آن انجام می‌شود.

مثال‌ها:

| ارز      | توضیح |
| -------- | ----- |
| `"IRT"`  | تومان |
| `"USDT"` | تتر   |

---

## `type`

```c
type=0
```

مشخص می‌کند سفارش خرید است یا فروش.

| مقدار | نوع سفارش   |
| ----- | ----------- |
| `"0"` | فروش (SELL) |
| `"1"` | خرید (BUY)  |

---

## `category_type`

```c
category_type=1
```

نوع اجرای سفارش را مشخص می‌کند.

| مقدار | نوع اجرا |
| ----- | -------- |
| `"0"` | Limit    |
| `"1"` | Market   |

---

# تفاوت سفارش Market و Limit

## سفارش Market

سفارش بلافاصله با قیمت فعلی بازار اجرا می‌شود.

```c
category_type=1
```

یعنی:

> همین الان با بهترین قیمت موجود بازار معامله انجام شود.

---

## سفارش Limit

شما قیمت دلخواه تعیین می‌کنید و سفارش فقط زمانی اجرا می‌شود که بازار به آن قیمت برسد.

```c
category_type=0
```

یعنی:

> سفارش فقط روی قیمت تعیین‌شده اجرا شود.

---

# پارامتر `amount`

```c
amount=2
```

مقدار ارزی که قصد خرید یا فروش آن را دارید.

مثال‌ها:

| مقدار   | توضیح   |
| ------- | ------- |
| `"2"`   | دو ADA  |
| `"0.5"` | نیم ADA |
| `"10"`  | ده ADA  |

---

# سفارش فروش Market

فایل:

```bash
2sell.c
```

تنظیمات:

```c
#define BASE   "ADA"
#define QUOTE  "IRT"
#define AMOUNT "2"
```

کامپایل:

```bash
gcc 2sell.c -o 2sell -lcurl -lssl -lcrypto
```

اجرا:

```bash
./2sell
```

این سفارش:

* 2 عدد ADA را
* با قیمت لحظه‌ای بازار
* فوراً می‌فروشد.

---

# سفارش خرید Market

فایل:

```bash
irtbuy.c
```

تنظیمات:

```c
#define BASE         "ADA"
#define QUOTE        "IRT"
#define SPEND_AMOUNT "200000"
```

کامپایل:

```bash
gcc irtbuy.c -o irtbuy -lcurl -lssl -lcrypto
```

اجرا:

```bash
./irtbuy
```

یعنی:

> با 200 هزار تومان ADA خریداری کن.

---

# سفارش خرید Limit

فایل:

```bash
2buy.c
```

تنظیمات:

```c
#define BASE   "ADA"
#define QUOTE  "IRT"
#define AMOUNT "2"
```

کامپایل:

```bash
gcc 2buy.c -o 2buy -lcurl -lssl -lcrypto
```

اجرا:

```bash
./2buy
```

این برنامه:

1. بهترین قیمت فروش را از Order Book دریافت می‌کند.
2. سفارش Limit Buy ثبت می‌کند.
3. پاسخ سفارش را نمایش می‌دهد.

---

# سفارش فروش Limit

نمونه پارامترها:

```c
amount=2
base_coin_symbol=ADA
category_type=0
price=120000
quote_coin_symbol=IRT
type=0
```

معنی:

> اگر قیمت ADA به 120 هزار تومان رسید، 2 عدد ADA فروخته شود.

---

# فروش کل موجودی ADA به‌صورت Market

فایل:

```bash
asell.c
```

تنظیمات:

```c
#define COIN_SYMBOL "ADA"
#define QUOTE       "IRT"
```

کامپایل:

```bash
gcc asell.c -o asell -lcurl -lssl -lcrypto
```

اجرا:

```bash
./asell
```

این برنامه:

1. موجودی کیف پول ADA را دریافت می‌کند.
2. مقدار Available Balance را پیدا می‌کند.
3. کل موجودی را به‌صورت Market Sell می‌فروشد.

---

# ساخت Signature در C

تمام سفارش‌ها باید با HMAC-SHA256 امضا شوند.

نمونه تابع:

```c
static void hmac_sha256_hex(const char *key,
                            const char *msg,
                            char *out_hex)
{
    unsigned char digest[32];
    unsigned int dlen = sizeof(digest);

    HMAC(
        EVP_sha256(),
        key,
        (int)strlen(key),
        (const unsigned char *)msg,
        strlen(msg),
        digest,
        &dlen
    );

    for (int i = 0; i < 32; i++)
        sprintf(out_hex + i * 2, "%02x", digest[i]);

    out_hex[64] = '\0';
}
```

---

# ساخت Query برای امضا

مثال فروش Market:

```c
amount=2
base_coin_symbol=ADA
category_type=1
quote_coin_symbol=IRT
type=0
```

رشته امضاشده:

```c
amount=2&base_coin_symbol=ADA&category_type=1&quote_coin_symbol=IRT&type=0
```

نکته مهم:

> پارامترها باید به‌ترتیب الفبایی مرتب شوند.

---

# جمع‌بندی سریع

| نوع سفارش | مقدار               |
| --------- | ------------------- |
| خرید      | `"type=1"`          |
| فروش      | `"type=0"`          |
| Market    | `"category_type=1"` |
| Limit     | `"category_type=0"` |

---

# فایل‌های آماده

| فایل       | عملکرد                    |
| ---------- | ------------------------- |
| `2buy.c`   | خرید Limit                |
| `2sell.c`  | فروش Market با مقدار مشخص |
| `asell.c`  | فروش کل موجودی            |
| `irtbuy.c` | خرید Market با مبلغ مشخص  |

---

# تفاوت اصلی BUY و SELL

## BUY Market

از پارامتر زیر استفاده می‌شود:

```c
quote_coin_amount
```

مقدار پولی که قصد خرج کردن آن را دارید.

مثال:

```c
quote_coin_amount=200000
```

---

## SELL Market

از پارامتر زیر استفاده می‌شود:

```c
amount
```

مقدار کوینی که قصد فروش آن را دارید.

مثال:

```c
amount=2
```
