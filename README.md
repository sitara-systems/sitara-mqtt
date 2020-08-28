# Sitara MQTT
A MQTT Client based off of [Eclipse Paho](https://www.eclipse.org/paho/). The MQTT protocol provides a lightweight method of carrying out messaging using a publish/subscribe model. This makes it suitable for Internet of Things messaging such as with low power sensors or mobile devices such as phones, embedded computers or microcontrollers.

## Requirements
You'll need to build both `paho.mqtt.c` and `paho.mqtt.cpp`.

1. Download OpenSSL [here](http://slproweb.com/products/Win32OpenSSL.html).  Install Win64 OpenSSL 1.1.1 full version, for developers.
2. Using CMake, build and compile `paho.mqtt.c`.
3. Using CMake, build and compile `paho.mqtt.cpp`.  Be sure to set the configuration to the `INSTALL` path for `paho.mqtt.c`; by default this was `C:\Program Files (x86)/Eclipse Paho C/`.

## Examples
* `MqttClientExample` shows a basic MQTT Client communicating with the Mosquitto test server at [http://test.mosquitto.org/](http://test.mosquitto.org/).
