<?php

$result = "";

if ($_SERVER["REQUEST_METHOD"] === "POST") {

    $apiKey = $_POST["api_key"] ?? "";
    $secretKey = $_POST["secret_key"] ?? "";

    $base = "ADA";
    $quote = "IRT";
    $amount = "2";

    $baseUrl = "https://rest.bit24.cash";

    function signParams($params, $secretKey)
    {
        unset($params["signature"]);
        ksort($params);

        $queryString = http_build_query($params, '', '&', PHP_QUERY_RFC3986);

        return hash_hmac(
            'sha256',
            $queryString,
            $secretKey
        );
    }

    function getBestAsk($apiKey, $baseUrl, $base, $quote)
    {
        $url = $baseUrl . "/pro/capi/v1/markets/order-books?" . http_build_query([
            "base_coin" => $base,
            "quote_coin" => $quote
        ]);

        $ch = curl_init($url);

        curl_setopt_array($ch, [
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_HTTPHEADER => [
                "Accept: application/json",
                "X-BIT24-APIKEY: $apiKey"
            ]
        ]);

        $response = curl_exec($ch);

        if (curl_errno($ch)) {
            throw new Exception(curl_error($ch));
        }

        curl_close($ch);

        $data = json_decode($response, true);

        if (!$data["success"]) {
            throw new Exception($data["error"] ?? "Order book error");
        }

        if (empty($data["data"]["sell_orders"])) {
            throw new Exception("No sell orders available");
        }

        return $data["data"]["sell_orders"][0]["price"];
    }

    function submitLimitBuyOrder(
        $apiKey,
        $secretKey,
        $baseUrl,
        $base,
        $quote,
        $amount,
        $price
    ) {

        $url = $baseUrl . "/pro/capi/v1/orders/submit";

        $params = [
            "base_coin_symbol" => $base,
            "quote_coin_symbol" => $quote,
            "type" => "1",
            "category_type" => "0",
            "price" => $price,
            "amount" => $amount
        ];

        $params["signature"] = signParams($params, $secretKey);

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

        if (curl_errno($ch)) {
            throw new Exception(curl_error($ch));
        }

        curl_close($ch);

        return json_decode($response, true);
    }

    try {

        $bestAsk = getBestAsk(
            $apiKey,
            $baseUrl,
            $base,
            $quote
        );

        $response = submitLimitBuyOrder(
            $apiKey,
            $secretKey,
            $baseUrl,
            $base,
            $quote,
            $amount,
            $bestAsk
        );

        if (!empty($response["success"])) {

            $order = $response["data"]["order"];

            $result =
                "Order placed successfully!\n" .
                "Order ID: " . $order["id"] . "\n" .
                "Status: " . $order["status_text"] . "\n" .
                "Price: " . $order["each_price"] . " IRT\n" .
                "Amount: " . $order["amount"] . " ADA\n" .
                "Total: " . $order["total"] . " IRT";

        } else {

            $result = "Order failed: " . ($response["error"] ?? "Unknown error");
        }

    } catch (Exception $e) {
        $result = "Error: " . $e->getMessage();
    }
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Bit24 ADA Buyer</title>
</head>
<body>

<h2>Bit24 ADA Buyer</h2>

<form method="post">

    <p>
        API Key<br>
        <input type="text" name="api_key" required>
    </p>

    <p>
        Secret Key<br>
        <input type="text" name="secret_key" required>
    </p>

    <button type="submit">
        Buy 2 ADA
    </button>

</form>

<pre><?php echo htmlspecialchars($result); ?></pre>

<script>
console.log("Bit24 ADA Buyer Loaded");
</script>

</body>
</html>