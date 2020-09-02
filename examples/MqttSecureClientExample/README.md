# Mqtt Secure Client Example

This example contains several options for secure encryption.  A rundown of them:

## Encrypted TCP, Server-Only Authenication
To create a secure SSL connection over TCP that only requires server authenication (e.g. a server certificate file), use:

```
mMqtt = sitara::paho::MqttClient::make("ssl://test.mosquitto.org:8883", "Sitara Systems Test Client");
mqtt::ssl_options sslOpts;
sslOpts.set_trust_store(ci::app::getAssetPath("ssl\\mosquitto.org.crt").string());
mMqtt->setSslOptions(sslOpts);
```

Mosquitto.org hosts a test server on port 8883 for just this purpose.  You can download the public certificate [here](http://test.mosquitto.org/ssl/mosquitto.org.crt), though it's also contained in `assets\ssl\`.

Please note that you MUST use the `ssl://` prefix in the URI, or ssl will not be enabled.

## Encrypted TCP, Requires Client Certificate
If a client certificate is needed, you can connect by pointing your client to your public certificate and private key:

```
// add public certificate
sslOpts.set_key_store(ci::app::getAssetPath("ssl\\client.pem").string());

// add private key
sslOpts.set_private_key(ci::app::getAssetPath("ssl\\client.key").string());
```

Mosquitto.org hosts a test server on port 8884 that requires a client certificate; your personalized client certificate can be generated [here](http://test.mosquitto.org/ssl/) if you'd like to test this functionality.  Client Certificates are only valid for 90 days.

## Encrypted Websockets, Requires Certificate
Mosquitto.org hosts a test server on port 8081 that utilizes WebSockets Secure with a Let's Encrypt certificate.  I haven't gotten around to testing this.
