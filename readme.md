# Bit24 Trading Bot 🤖

<div dir="rtl">

# ربات معامله‌گر Bit24 با Python، Go، Node.js و PHP

### معرفی پروژه

این پروژه مجموعه‌ای از ابزارهای ساده و کاربردی برای انجام معاملات خودکار در صرافی **Bit24** می‌باشد.

نسخه‌های پروژه با:

* **Python**
* **Go (Golang)**
* **Node.js**
* **PHP**

ارائه شده‌اند تا بتوانید بر اساس نیاز خود از هر زبان استفاده کنید.

با استفاده از این کدها می‌توانید:

* سفارش خرید و فروش ثبت کنید
* سفارش مارکت یا لیمیت ارسال کنید
* تمام موجودی یک ارز را بفروشید
* خرید با مبلغ تومانی انجام دهید
* ساختار API صرافی Bit24 را یاد بگیرید
* SDK یا Trading Bot حرفه‌ای توسعه دهید

---

# قابلیت‌ها

| قابلیت              | توضیح                         |
| ------------------- | ----------------------------- |
| 🟢 خرید مارکت       | خرید فوری با مبلغ تومانی مشخص |
| 🔴 فروش مارکت       | فروش فوری مقدار مشخصی از ارز  |
| 🟡 خرید لیمیت       | خرید در قیمت دلخواه           |
| 🔵 فروش لیمیت       | فروش در قیمت دلخواه           |
| 📦 فروش تمام موجودی | فروش کل موجودی یک ارز         |
| ⚡ نسخه Go          | اجرای سریع‌تر و حرفه‌ای‌تر    |
| 🐍 نسخه Python      | ساده و مناسب یادگیری          |
| 🟩 نسخه Node.js     | مناسب SDK و Bot های مدرن      |
| 🐘 نسخه PHP         | مناسب هاست‌های اشتراکی و Backend ساده |

---

# ساختار پروژه

```text
Bit24-Trading-Bot/
│
├── python/
│   ├── 2buy.py
│   ├── 2sell.py
│   ├── asell.py
│   ├── irtbuy.py
│   └── API.md
│
├── go/
│   ├── 2buy.go
│   ├── 2sell.go
│   ├── asell.go
│   ├── irtbuy.go
│   └── API.md
│
├── nodejs/
│   ├── 2buy.js
│   ├── 2sell.js
│   ├── asell.js
│   ├── irtbuy.js
│   └── API.md
│
├── php/
│   ├── 2buy.php
│   ├── 2sell.php
│   ├── asell.php
│   ├── irtbuy.php
│   └── API.md
│
├── bot/
│   └── rule.md
│   └── Readme.md
│
└── README.md
```

---

# فایل‌های Python

| فایل        | کاربرد                        |
| ----------- | ----------------------------- |
| `2buy.py`   | خرید 2 واحد ارز (Limit Buy)   |
| `2sell.py`  | فروش 2 واحد ارز (Market Sell) |
| `asell.py`  | فروش تمام موجودی ارز          |
| `irtbuy.py` | خرید با مبلغ تومانی           |
| `API.md`    | مستندات کامل Python API       |

---

# فایل‌های Go

| فایل                  | کاربرد                 |
| --------------------- | ---------------------- |
| `2buy.go`             | خرید لیمیت با Go       |
| `2sell.go`            | فروش مارکت با Go       |
| `sell_all_balance.go` | فروش کل موجودی با Go   |
| `irtbuy.go`           | خرید با مبلغ IRT با Go |
| `API_GO.md`           | مستندات کامل API در Go |

---

# فایل‌های Node.js

| فایل            | کاربرد                      |
| --------------- | --------------------------- |
| `2buy.js`       | خرید لیمیت با Node.js       |
| `2sell.js`      | فروش مارکت با Node.js       |
| `asell.js`      | فروش کل موجودی با Node.js   |
| `irtbuy.js`     | خرید با مبلغ تومانی         |
| `API_NODEJS.md` | مستندات کامل API در Node.js |

---

# فایل‌های PHP

| فایل         | کاربرد                         |
| ------------ | ------------------------------ |
| `2buy.php`   | خرید لیمیت ۲ واحد ADA          |
| `2sell.php`  | فروش مارکت ۲ واحد ADA          |
| `asell.php`  | فروش تمام موجودی ADA (مارکت)  |
| `irtbuy.php` | خرید مارکت با مبلغ تومانی (۲۰۰ هزار تومان) |
| `API_PHP.md` | مستندات کامل API در PHP        |

---

# چرا PHP؟

نسخه PHP برای افرادی مناسب است که:

* روی هاست‌های اشتراکی (Shared Hosting) کار می‌کنند
* نیاز به یک Webhook ساده یا Backend سبک دارند
* می‌خواهند بدون کامپایل یا نصب وابستگی‌های سنگین، ربات معاملاتی راه‌اندازی کنند
* با PHP آشنایی دارند و می‌خواهند سریعترین نمونه اولیه (MVP) را بسازند

---

# پیش‌نیازهای PHP

- PHP نسخه ۷.۴ یا بالاتر (توصیه ۸.x)
- افزونه cURL فعال
- دسترسی به تابع‌های `hash_hmac` و `http_build_query`

بررسی PHP:

```bash
php -v
php -m | grep curl
```

---

# اجرای فایل‌های PHP

روش ۱ – اجرا با PHP Built-in Server (برای تست محلی):

```bash
php -S localhost:8000
```

سپس در مرورگر آدرس `http://localhost:8000/2buy.php` را باز کنید.

روش ۲ – اجرا به صورت CLI (Command Line):

```bash
php 2buy.php
```

> توجه: در حالت CLI باید ورودی API Key و Secret Key را از طریق stdin دریافت کنید یا داخل کد مقداردهی کنید. اسکریپت‌های ارائه شده از طریق فرم HTML کار می‌کنند، برای CLI می‌توانید آنها را ساده‌سازی کنید.

---

# ساختار params در PHP

```php
$params = [
    "base_coin_symbol" => "ADA",
    "quote_coin_symbol" => "IRT",
    "type" => "1",
    "category_type" => "1",
    "quote_coin_amount" => "200000"
];
```

---

# ساخت Signature در PHP

```php
function signParams($params, $secretKey)
{
    unset($params["signature"]);
    ksort($params);
    $queryString = http_build_query($params, '', '&', PHP_QUERY_RFC3986);
    return hash_hmac('sha256', $queryString, $secretKey);
}
```

---

# ارسال درخواست در PHP (cURL)

```php
$ch = curl_init($url);
curl_setopt_array($ch, [
    CURLOPT_POST => true,
    CURLOPT_RETURNTRANSFER => true,
    CURLOPT_POSTFIELDS => http_build_query($params),
    CURLOPT_HTTPHEADER => [
        "Accept: application/json",
        "Content-Type: application/x-www-form-urlencoded",
        "X-BIT24-APIKEY: $apiKey"
    ]
]);
$response = curl_exec($ch);
```

---

# نحوه استفاده

## 1. خرید ۲ واحد ارز (Limit Buy)

### Python
```bash
python 2buy.py
```
### Go
```bash
go run 2buy.go
```
### Node.js
```bash
node 2buy.js
```
### PHP
```bash
php -S localhost:8000
# سپس مرورگر -> http://localhost:8000/2buy.php
```

---

## 2. فروش ۲ واحد ارز (Market Sell)

### Python
```bash
python 2sell.py
```
### Go
```bash
go run 2sell.go
```
### Node.js
```bash
node 2sell.js
```
### PHP
```bash
php -S localhost:8000
# مرورگر -> http://localhost:8000/2sell.php
```

---

## 3. فروش تمام موجودی

### Python
```bash
python asell.py
```
### Go
```bash
go run sell_all_balance.go
```
### Node.js
```bash
node asell.js
```
### PHP
```bash
php -S localhost:8000
# مرورگر -> http://localhost:8000/asell.php
```

---

## 4. خرید با مبلغ تومانی (Market Buy)

### Python
```bash
python irtbuy.py
```
### Go
```bash
go run irtbuy.go
```
### Node.js
```bash
node irtbuy.js
```
### PHP
```bash
php -S localhost:8000
# مرورگر -> http://localhost:8000/irtbuy.php
```

---

# تغییر ارز و مقدار

## Python
```python
params = {
    "base_coin_symbol": "ADA",
    "quote_coin_symbol": "IRT",
    "amount": "2",
}
```

## Go
```go
params := map[string]string{
    "base_coin_symbol":  "ADA",
    "quote_coin_symbol": "IRT",
    "amount":            "2",
}
```

## Node.js
```js
const params = {
    base_coin_symbol: "ADA",
    quote_coin_symbol: "IRT",
    amount: "2"
};
```

## PHP
```php
$params = [
    "base_coin_symbol" => "ADA",
    "quote_coin_symbol" => "IRT",
    "amount" => "2"
];
```

---

# مثال‌های عملی

## فروش لیمیت ADA

### Python
```python
params = {
    "base_coin_symbol": "ADA",
    "quote_coin_symbol": "IRT",
    "type": "0",
    "category_type": "0",
    "price": "100000",
    "amount": "5"
}
```

### Go
```go
params := map[string]string{
    "base_coin_symbol":  "ADA",
    "quote_coin_symbol": "IRT",
    "type":              "0",
    "category_type":     "0",
    "price":             "100000",
    "amount":            "5",
}
```

### Node.js
```js
const params = {
    base_coin_symbol: "ADA",
    quote_coin_symbol: "IRT",
    type: "0",
    category_type: "0",
    price: "100000",
    amount: "5"
};
```

### PHP
```php
$params = [
    "base_coin_symbol" => "ADA",
    "quote_coin_symbol" => "IRT",
    "type" => "0",
    "category_type" => "0",
    "price" => "100000",
    "amount" => "5"
];
```

---

## خرید اتریوم با ۱ میلیون تومان

### Python
```python
params = {
    "base_coin_symbol": "ETH",
    "quote_coin_symbol": "IRT",
    "type": "1",
    "category_type": "1",
    "quote_coin_amount": "1000000"
}
```

### Go
```go
params := map[string]string{
    "base_coin_symbol":  "ETH",
    "quote_coin_symbol": "IRT",
    "type":              "1",
    "category_type":     "1",
    "quote_coin_amount": "1000000",
}
```

### Node.js
```js
const params = {
    base_coin_symbol: "ETH",
    quote_coin_symbol: "IRT",
    type: "1",
    category_type: "1",
    quote_coin_amount: "1000000"
};
```

### PHP
```php
$params = [
    "base_coin_symbol" => "ETH",
    "quote_coin_symbol" => "IRT",
    "type" => "1",
    "category_type" => "1",
    "quote_coin_amount" => "1000000"
];
```

---

# جدول مقایسه کامل زبان‌ها

| ویژگی / معیار                          | 🐍 Python       | 🚀 Go (Golang)  | 🟩 Node.js      | 🐘 PHP          |
| -------------------------------------- | --------------- | --------------- | --------------- | --------------- |
| **سرعت اجرا**                          | متوسط           | خیلی بالا       | بالا (V8)       | متوسط تا بالا   |
| **همزمانی (Concurrency)**              | محدود (GIL)     | عالی (Goroutine)| عالی (async)    | محدود (پردازش درخواست‌های همزمان نیازمند swoole) |
| **مناسب برای ربات معاملاتی Real-time** | خوب (با asyncio)| عالی            | عالی            | متوسط (بیشتر برای Cron/Webhook) |
| **ساخت API / وب‌سرویس**                | خوب (Flask/Django)| عالی (net/http)| عالی (Express)  | عالی (Native)   |
| **مناسب برای هاست اشتراکی**            | خیر             | خیر             | خیر             | **بله**         |
| **یادگیری و ساده‌گی برای مبتدی**        | عالی            | متوسط           | خوب             | خوب             |
| **نصب وابستگی‌ها**                     | pip             | ماژولار (بدون وابستگی زیاد) | npm             | بدون نیاز (فعال کردن ext) |
| **خروجی تک‌فایل اجرایی (EXE)**         | نیاز به PyInstaller | ✅ بومی (go build) | نیاز به pkg/nexe | خیر (نیاز به PHP interpreter) |
| **مدیریت حافظه**                       | خودکار (GC)     | خودکار (GC بهینه) | خودکار (GC)     | خودکار (در پایان هر درخواست) |
| **مناسب برای SDK / کتابخانه**          | خیلی خوب        | عالی            | عالی            | متوسط           |

---

# جمع‌بندی نهایی

⚡ این پروژه می‌تواند پایه‌ای برای ساخت:

* Trading Bot حرفه‌ای
* CLI Tool
* Go SDK
* Node.js SDK
* TypeScript SDK
* PHP Webhook یا Cron Job
* Market Scanner
* Auto Trader
* Signal Bot
* Arbitrage System

باشد.

</div>

---

# English Summary

Bit24 Trading Bot supports:

* Python
* Go (Golang)
* Node.js
* PHP

implementations for automated trading on Bit24 exchange.

---

# Features

* ✅ Market Buy
* ✅ Market Sell
* ✅ Limit Buy
* ✅ Limit Sell
* ✅ Sell Full Balance
* ✅ HMAC-SHA256 Signing
* ✅ Python Examples
* ✅ Go Examples
* ✅ Node.js Examples
* ✅ PHP Examples

---

# Project Structure

```text
Bit24-Trading-Bot/
├── python/
├── go/
├── nodejs/
├── php/
├── bot/
└── README.md
```

---

# Quick Start

## Python
```bash
pip install requests
python 2buy.py
```

## Go
```bash
go run 2buy.go
go build
```

## Node.js
```bash
node 2buy.js
```

## PHP
```bash
php -S localhost:8000
# Open http://localhost:8000/2buy.php
```

---

# PHP Prerequisites

- PHP 7.4+ (8.x recommended)
- cURL extension enabled
- `hash_hmac` and `http_build_query` functions

Check:
```bash
php -v
php -m | grep curl
```

---

# Signature Generation in PHP

```php
function signParams($params, $secretKey) {
    unset($params["signature"]);
    ksort($params);
    $queryString = http_build_query($params, '', '&', PHP_QUERY_RFC3986);
    return hash_hmac('sha256', $queryString, $secretKey);
}
```

---

# PHP Files Description

| File         | Description                           |
| ------------ | ------------------------------------- |
| `2buy.php`   | Limit buy 2 ADA (uses best ask price) |
| `2sell.php`  | Market sell 2 ADA                     |
| `asell.php`  | Market sell all ADA balance           |
| `irtbuy.php` | Market buy with 200,000 IRT           |
| `API_PHP.md` | Full PHP API documentation            |

---

# Supported Examples

| File     | Description          |
| -------- | -------------------- |
| `2buy`   | Limit Buy            |
| `2sell`  | Market Sell          |
| `asell`  | Sell Full Balance    |
| `irtbuy` | Buy Using IRT Amount |

---

# Supported Trading Types

| Type | Value |
| ---- | ----- |
| Buy  | `"1"` |
| Sell | `"0"` |

| Order Type | Value |
| ---------- | ----- |
| Market     | `"1"` |
| Limit      | `"0"` |

---

# Comprehensive Language Comparison Table

| Feature / Metric                     | 🐍 Python        | 🚀 Go (Golang)   | 🟩 Node.js       | 🐘 PHP           |
| ------------------------------------ | ---------------- | ---------------- | ---------------- | ---------------- |
| Execution Speed                      | Medium           | Very High        | High (V8)        | Medium to High   |
| Concurrency                          | Limited (GIL)    | Excellent (Goroutines) | Excellent (async) | Limited (per‑request) |
| Real‑time Trading Bot                | Good (asyncio)   | Excellent        | Excellent        | Medium (Cron/Webhook) |
| API / Web Service Development        | Good (Flask/Django) | Excellent (net/http) | Excellent (Express) | Excellent (native) |
| Shared Hosting Friendly              | No               | No               | No               | **Yes**          |
| Learning Curve for Beginners         | Excellent        | Medium           | Good             | Good             |
| Dependency Management                | pip              | Minimal (no heavy deps) | npm            | None (just enable ext) |
| Single Executable Output             | Need PyInstaller | ✅ Native `go build` | Need pkg/nexe   | No (needs PHP interpreter) |
| Memory Management                    | Automatic (GC)   | Automatic (optimized GC) | Automatic (GC) | Automatic (per request) |
| Suitable for SDK / Library           | Very Good        | Excellent        | Excellent        | Medium           |

---

# Security Notice

* API Keys are processed only on your system
* No data is sent anywhere else
* Never share your Secret Key
* Test with small amounts first

---

# Final Goal

This project can become a base for:

* Trading Bots
* SDK Development (Python, Go, Node.js, PHP)
* CLI Tools
* Auto Traders
* Signal Bots
* Arbitrage Systems
* Market Scanners
* Webhook Receivers (PHP)

MIT License
