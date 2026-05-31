# Bit24 PHP Examples

مجموعه‌ای از نمونه‌کدهای PHP برای کار با API صرافی Bit24.

## فایل‌ها

### Buy Limit

خرید مقدار مشخصی از یک ارز با قیمت سفارش فروش اول بازار (Best Ask).

نمونه:

* خرید 2 ADA
* ثبت سفارش Limit Buy

---

### Sell Market

فروش مقدار مشخصی از یک ارز به قیمت بازار.

نمونه:

* فروش 2 ADA
* ثبت سفارش Market Sell

---

### Sell All Balance

فروش تمام موجودی یک ارز به قیمت بازار.

نمونه:

* فروش کل موجودی ADA
* دریافت موجودی کیف پول
* ثبت سفارش فروش کل موجودی

---

### Buy Market By Amount

خرید یک ارز با مقدار مشخصی از ارز پرداختی.

نمونه:

* خرید ADA با 200000 IRT
* ثبت سفارش Market Buy

---

# پیش‌نیازها

PHP 8 یا جدیدتر

فعال بودن افزونه cURL

برای بررسی:

```bash
php -m | grep curl
```

---

# اجرا

وارد پوشه پروژه شوید:

```bash
cd project
```

اجرای وب سرور داخلی PHP:

```bash
php -S localhost:8000
```

سپس در مرورگر باز کنید:

```text
http://localhost:8000
```

---

# وارد کردن API

پس از باز شدن صفحه:

1. API Key را وارد کنید.
2. Secret Key را وارد کنید.
3. دکمه اجرا را بزنید.

---

# تغییر ارز

در هر فایل PHP بخش زیر را پیدا کنید:

```php
"base_coin_symbol" => "ADA",
"quote_coin_symbol" => "IRT"
```

مثال:

```php
"base_coin_symbol" => "BTC",
"quote_coin_symbol" => "IRT"
```

یا:

```php
"base_coin_symbol" => "DOGE",
"quote_coin_symbol" => "USDT"
```

---

# تغییر مقدار خرید

در فایل Market Buy:

```php
"quote_coin_amount" => "200000"
```

مثال:

```php
"quote_coin_amount" => "500000"
```

یعنی 500 هزار تومان از موجودی IRT خرج شود.

---

# تغییر مقدار فروش

در فایل Sell Market:

```php
"amount" => "2"
```

مثال:

```php
"amount" => "10"
```

یعنی 10 واحد از ارز انتخابی فروخته شود.

---

# فروش کل موجودی

در فایل Sell All Balance نیازی به تعیین مقدار نیست.

برنامه:

1. موجودی کیف پول را دریافت می‌کند.
2. موجودی قابل معامله ارز انتخابی را پیدا می‌کند.
3. کل موجودی را به صورت Market Sell می‌فروشد.

---

# امنیت

هرگز API Key و Secret Key خود را در GitHub منتشر نکنید.

فایل‌های حاوی کلیدهای API را Commit نکنید.

از دسترسی فقط Trading برای API استفاده کنید.

---

# ساختار سفارش‌ها

## Market Buy

```php
"type" => "1",
"category_type" => "1"
```

## Market Sell

```php
"type" => "0",
"category_type" => "1"
```

## Limit Buy

```php
"type" => "1",
"category_type" => "0"
```

---

# مسئولیت استفاده

این پروژه صرفاً نمونه آموزشی برای استفاده از API صرافی Bit24 است.

تمام مسئولیت معاملات، سود و زیان بر عهده کاربر خواهد بود.


# Bit24 PHP Examples

Simple PHP examples for Bit24 Exchange API.

## Files

* `buy_limit.php` → Buy a fixed amount at best ask price.
* `buy_market.php` → Buy using a fixed IRT amount.
* `sell_market.php` → Sell a fixed coin amount.
* `sell_all.php` → Sell the entire balance of a selected coin.

## Requirements

* PHP 8+
* PHP cURL extension enabled

## Run

```bash
php -S localhost:8000
```

Open:

```text
http://localhost:8000
```

## API Keys

Enter your:

* API Key
* Secret Key

in the web form and submit.

## Edit Coin

Find:

```php
"base_coin_symbol" => "ADA",
"quote_coin_symbol" => "IRT"
```

Example:

```php
"base_coin_symbol" => "BTC",
"quote_coin_symbol" => "IRT"
```

## Edit Amount

Buy Market:

```php
"quote_coin_amount" => "200000"
```

Sell Market:

```php
"amount" => "2"
```

## Security

Never publish your API Key or Secret Key.

## Disclaimer

These examples are provided for educational purposes only. Use at your own risk.
