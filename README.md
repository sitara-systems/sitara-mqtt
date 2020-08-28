# Sitara MQTT
A MQTT Client based off of [Eclipse Mosquitto](http://mosquitto.org). The MQTT protocol provides a lightweight method of carrying out messaging using a publish/subscribe model. This makes it suitable for Internet of Things messaging such as with low power sensors or mobile devices such as phones, embedded computers or microcontrollers.

## Requirements
Install the latest version of [Mosquitto](https://mosquitto.org/download/).  Current version as of the last commit is 1.6.12.

After Mosquitto is installed, update the property sheet included in `config/` to the path of the header and library files.  By default, these should be at `C:\Program Files\mosquitto\devel`.

## Examples
* `MqttExample` shows a basic MQTT Client communicating with the Mosquitto test server at [http://test.mosquitto.org/](http://test.mosquitto.org/).
