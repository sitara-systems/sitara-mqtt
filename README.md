# Sitara MQTT
A MQTT Client based off of [Eclipse Paho](https://www.eclipse.org/paho/). The MQTT protocol provides a lightweight method of carrying out messaging using a publish/subscribe model. This makes it suitable for Internet of Things messaging such as with low power sensors or mobile devices such as phones, embedded computers or microcontrollers.

## Requirements
* paho.mqtt.c (included)
* paho.mqtt.cpp (included)

## Examples
* `MqttClientExample` shows a basic MQTT Client communicating with the Mosquitto test server at [http://test.mosquitto.org/](http://test.mosquitto.org/).

## Certificate Authority
TLS Certificates are from the [Mozilla CA Certificate Store](https://www.mozilla.org/en-US/about/governance/policies/security-group/certs/).  The file was downloaded from [curl](https://curl.haxx.se/docs/caextract.html) (to get it in a compatible .pem format) on 22 July 2020.

Inside of your cinder app, you can add the path to the file using `ci::app::addAssetDirectory()`.
