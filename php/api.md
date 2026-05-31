# آموزش کامل سفارش‌گذاری در API صرافی Bit24 (PHP)

## ساختار کلی سفارش در Bit24

تمام سفارش‌ها در Bit24 با استفاده از آرایه `$params` ارسال می‌شوند.

نمونه کلی:

```php
$params = [
    "base_coin_symbol" => "ADA",
    "quote_coin_symbol" => "IRT",
    "type" => "0",
    "category_type" => "1",
    "amount" => "2"
];
```

---

# توضیح کامل پارامترها

## base_coin_symbol

```php
"base_coin_symbol" => "ADA"
```

ارز اصلی‌ای که قصد خرید یا فروش آن را دارید.

| ارز      | مقدار   |
| -------- | ------- |
| بیت کوین | `"BTC"` |
| اتریوم   | `"ETH"` |
| کاردانو  | `"ADA"` |
| سولانا   | `"SOL"` |

---

## quote_coin_symbol

```php
"quote_coin_symbol" => "IRT"
```

ارزی که معامله بر اساس آن انجام می‌شود.

| ارز      | توضیح |
| -------- | ----- |
| `"IRT"`  | تومان |
| `"USDT"` | تتر   |

---

## type

```php
"type" => "0"
```

نوع سفارش را مشخص می‌کند.

| مقدار | نوع سفارش   |
| ----- | ----------- |
| `"0"` | فروش (SELL) |
| `"1"` | خرید (BUY)  |

---

## category_type

```php
"category_type" => "1"
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

```php
"category_type" => "1"
```

یعنی:

> همین الان با بهترین قیمت موجود بازار معامله انجام شود.

---

## سفارش Limit

سفارش فقط زمانی اجرا می‌شود که قیمت بازار به قیمت تعیین‌شده شما برسد.

```php
"category_type" => "0"
```

یعنی:

> سفارش روی قیمت مشخص‌شده ثبت شود.

---

# پارامتر amount

```php
"amount" => "2"
```

مقدار ارزی که قصد خرید یا فروش آن را دارید.

| مقدار   | توضیح   |
| ------- | ------- |
| `"2"`   | دو ADA  |
| `"0.5"` | نیم ADA |
| `"10"`  | ده ADA  |

---

# سفارش فروش Market

## فروش فوری با قیمت بازار

```php
$params = [
    "base_coin_symbol" => "ADA",
    "quote_coin_symbol" => "IRT",

    "type" => "0",
    "category_type" => "1",

    "amount" => "2"
];
```

این سفارش:

* 2 واحد ADA را
* با قیمت لحظه‌ای بازار
* فوراً می‌فروشد

---

# سفارش خرید Market

در خرید Market به جای amount باید مقدار پول پرداختی مشخص شود.

```php
$params = [
    "base_coin_symbol" => "ADA",
    "quote_coin_symbol" => "IRT",

    "type" => "1",
    "category_type" => "1",

    "quote_coin_amount" => "200000"
];
```

یعنی:

> با 200 هزار تومان ADA خریداری شود.

---

# سفارش خرید Limit

```php
$params = [
    "base_coin_symbol" => "ADA",
    "quote_coin_symbol" => "IRT",

    "type" => "1",
    "category_type" => "0",

    "price" => "95000",
    "amount" => "2"
];
```

یعنی:

> اگر قیمت ADA به 95 هزار تومان رسید، 2 ADA خریداری شود.

---

# سفارش فروش Limit

```php
$params = [
    "base_coin_symbol" => "ADA",
    "quote_coin_symbol" => "IRT",

    "type" => "0",
    "category_type" => "0",

    "price" => "120000",
    "amount" => "2"
];
```

یعنی:

> اگر قیمت ADA به 120 هزار تومان رسید، 2 ADA فروخته شود.

---

# مثال واقعی

فرض کنید قیمت فعلی ADA برابر باشد با:

```php
100000
```

تومان.

اگر وارد کنید:

```php
"price" => "95000"
```

یعنی:

> زمانی که قیمت به 95 هزار تومان رسید خرید انجام شود.

---

اگر وارد کنید:

```php
"price" => "120000"
```

یعنی:

> زمانی که قیمت به 120 هزار تومان رسید فروش انجام شود.

---

# ساخت امضای درخواست (Signature)

تمام سفارش‌ها باید با HMAC SHA256 امضا شوند.

```php
ksort($params);

$queryString = "";

foreach ($params as $key => $value) {

    if ($queryString !== "") {
        $queryString .= "&";
    }

    $queryString .= $key . "=" . $value;
}

$signature = hash_hmac(
    "sha256",
    $queryString,
    $secretKey
);

$params["signature"] = $signature;
```

---

# ارسال سفارش

```php
$headers = [
    "Accept: application/json",
    "Content-Type: application/x-www-form-urlencoded",
    "X-BIT24-APIKEY: $apiKey"
];

$ch = curl_init($url);

curl_setopt_array($ch, [
    CURLOPT_POST => true,
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_POSTFIELDS => http_build_query($params),
    CURLOPT_HTTPHEADER => $headers
]);

$response = curl_exec($ch);

curl_close($ch);
```

---

# جمع‌بندی سریع

| نوع سفارش | مقدار                    |
| --------- | ------------------------ |
| خرید      | `"type" => "1"`          |
| فروش      | `"type" => "0"`          |
| Market    | `"category_type" => "1"` |
| Limit     | `"category_type" => "0"` |

---

# تفاوت اصلی BUY و SELL

## BUY Market

```php
"quote_coin_amount"
```

مقدار پولی که می‌خواهید خرج کنید.

مثال:

```php
"quote_coin_amount" => "200000"
```

---

## SELL Market

```php
"amount"
```

مقدار کوینی که می‌خواهید بفروشید.

مثال:

```php
"amount" => "2"
```

---

# فایل‌های نمونه موجود

| فایل            | توضیح                  |
| --------------- | ---------------------- |
| buy_limit.php   | خرید Limit             |
| buy_market.php  | خرید Market            |
| sell_market.php | فروش Market            |
| sell_all.php    | فروش کل موجودی         |
| index.php       | نسخه تحت وب HTML + PHP |

---

# اجرا

اجرای سرور PHP:

```bash
php -S localhost:8000
```

سپس:

```text
http://localhost:8000
```

# Bit24 PHP Examples

Simple PHP examples for the Bit24 Exchange API.

## Files

* `buy_limit.php` — Limit Buy Order
* `buy_market.php` — Market Buy Order
* `sell_market.php` — Market Sell Order
* `sell_all.php` — Sell Full Balance

## Requirements

* PHP 8+
* cURL extension enabled

## Run

```bash
php -S localhost:8000
```

Open:

```text
http://localhost:8000
```

## Edit Coin

```php
"base_coin_symbol" => "ADA",
"quote_coin_symbol" => "IRT"
```

Example:

```php
"base_coin_symbol" => "BTC",
"quote_coin_symbol" => "USDT"
```

## Edit Amount

Market Sell:

```php
"amount" => "2"
```

Market Buy:

```php
"quote_coin_amount" => "200000"
```

Limit Buy/Sell:

```php
"price" => "95000",
"amount" => "2"
```

## Security

Never share your API Key or Secret Key.

## License

Educational examples for Bit24 API usage.
