#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Mqtt.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MqttSecureClientExampleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::shared_ptr<sitara::paho::MqttClient> mMqtt;
};

void MqttSecureClientExampleApp::setup() {
	ci::app::setFrameRate(60);

	// connect to mosquitto.org test server using port 8883 (TCP transport layer w/ SSL)
	mMqtt = sitara::paho::MqttClient::make("test.mosquitto.org:8883", "Sitara Systems Test Client");

	mqtt::ssl_options sslOpts;

	sslOpts.set_trust_store(ci::app::getAssetPath("ssl\\mosquitto.org.crt").string());
	sslOpts.set_key_store("client.pem");

	mMqtt->setSslOptions(sslOpts);

	mMqtt->setOnConnectHandler([&](const std::string& cause) {
		std::string topic = "sitara-systems";
		int qualityOfService = 0;

		std::cout << "\nSubscribing to topic '" << topic << "'\n"
			<< "\tfor client " << mMqtt->getClientName()
			<< " using QoS " << qualityOfService << "\n" << std::endl;

		mMqtt->getClient()->subscribe(topic, qualityOfService);
		});

	mMqtt->start();
}

void MqttSecureClientExampleApp::mouseDown( MouseEvent event ) {
}

void MqttSecureClientExampleApp::update() {
	if (ci::app::getElapsedFrames() % 300 == 0) {
		mMqtt->publish("sitara-systems", std::to_string(ci::app::getElapsedFrames()));
	}
}

void MqttSecureClientExampleApp::draw() {
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( MqttSecureClientExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {settings->setConsoleWindowEnabled(); })
