<?php

$result = "";

if ($_SERVER["REQUEST_METHOD"] === "POST") {

    $apiKey = $_POST["api_key"] ?? "";
    $secretKey = $_POST["secret_key"] ?? "";

    try {

        // =========================
        // CONFIG
        // =========================
        $coin = "ADA";
        $quote = "IRT";

        // =========================
        // GET WALLET BALANCE
        // =========================
        $walletUrl = "https://rest.bit24.cash/asset/capi/v1/wallet/assets";

        $walletHeaders = [
            "Accept: application/json",
            "X-BIT24-APIKEY: $apiKey"
        ];

        $walletParams = [
            "without_irt" => "0",
            "without_zero" => "1"
        ];

        $ch = curl_init(
            $walletUrl . "?" . http_build_query($walletParams)
        );

        curl_setopt_array($ch, [
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_HTTPHEADER => $walletHeaders
        ]);

        $walletResponse = curl_exec($ch);

        if (curl_errno($ch)) {
            throw new Exception(curl_error($ch));
        }

        $walletStatus = curl_getinfo($ch, CURLINFO_HTTP_CODE);

        curl_close($ch);

        if ($walletStatus != 200) {
            throw new Exception("Wallet request failed");
        }

        $walletData = json_decode($walletResponse, true);

        $assets = $walletData["data"]["asset"] ?? [];

        $availableBalance = 0;

        foreach ($assets as $asset) {

            if (($asset["symbol"] ?? "") === $coin) {

                $availableBalance = $asset["available_balance"];
                break;
            }
        }

        if ((float)$availableBalance <= 0) {
            throw new Exception("No {$coin} available to sell.");
        }

        // =========================
        // SELL ALL BALANCE
        // =========================
        $sellUrl = "https://rest.bit24.cash/pro/capi/v1/orders/submit";

        $params = [
            "base_coin_symbol" => $coin,
            "quote_coin_symbol" => $quote,
            "type" => "0",
            "category_type" => "1",
            "amount" => (string)$availableBalance
        ];

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

        $sellHeaders = [
            "Accept: application/json",
            "Content-Type: application/x-www-form-urlencoded",
            "X-BIT24-APIKEY: $apiKey"
        ];

        $ch = curl_init($sellUrl);

        curl_setopt_array($ch, [
            CURLOPT_POST => true,
            CURLOPT_RETURNTRANSFER => true,
            CURLOPT_POSTFIELDS => http_build_query($params),
            CURLOPT_HTTPHEADER => $sellHeaders
        ]);

        $sellResponse = curl_exec($ch);

        if (curl_errno($ch)) {
            throw new Exception(curl_error($ch));
        }

        $sellStatus = curl_getinfo($ch, CURLINFO_HTTP_CODE);

        curl_close($ch);

        $result .= "Coin: {$coin}\n";
        $result .= "Available Balance: {$availableBalance}\n\n";
        $result .= "HTTP Status: {$sellStatus}\n\n";
        $result .= $sellResponse;

    } catch (Exception $e) {

        $result = "Error: " . $e->getMessage();
    }
}

?>

<!DOCTYPE html>
<html>
<head>
    <title>Bit24 Sell All Balance</title>
</head>
<body>

<h2>Bit24 Sell All Coin Balance</h2>

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
        Sell All ADA
    </button>

</form>

<pre><?php echo htmlspecialchars($result); ?></pre>

<script>
console.log("Bit24 Sell All Loaded");
</script>

</body>
</html>