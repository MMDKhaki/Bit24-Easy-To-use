<?php

$result = "";

if ($_SERVER["REQUEST_METHOD"] === "POST") {

    $apiKey = $_POST["api_key"] ?? "";
    $secretKey = $_POST["secret_key"] ?? "";

    $url = "https://rest.bit24.cash/pro/capi/v1/orders/submit";

    $params = [
        "base_coin_symbol" => "ADA",
        "quote_coin_symbol" => "IRT",

        // SELL
        "type" => "0",

        // MARKET
        "category_type" => "1",

        // Amount of ADA to sell
        "amount" => "2"
    ];

    ksort($params);

    $query = http_build_query(
        $params,
        '',
        '&',
        PHP_QUERY_RFC3986
    );

    $signature = hash_hmac(
        'sha256',
        $query,
        $secretKey
    );

    $params["signature"] = $signature;

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
    $status = curl_getinfo($ch, CURLINFO_HTTP_CODE);

    if (curl_errno($ch)) {
        $result = "cURL Error: " . curl_error($ch);
    } else {
        $result =
            "HTTP Status: " . $status . "\n\n" .
            $response;
    }

    curl_close($ch);
}

?>

<!DOCTYPE html> 
<html>
<head>
    <title>Bit24 ADA Seller</title>
</head>
<body>

<h2>Bit24 Market Sell ADA</h2>

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
        Sell 2 ADA
    </button>

</form>

<pre><?php echo htmlspecialchars($result); ?></pre>

<script>
console.log("Bit24 Seller Loaded");
</script>

</body>
</html>