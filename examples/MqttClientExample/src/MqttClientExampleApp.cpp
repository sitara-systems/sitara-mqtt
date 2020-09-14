#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Mqtt.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MqttClientExampleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    std::shared_ptr<sitara::paho::MqttClient> mMqtt;
};

void MqttClientExampleApp::setup() {
	ci::app::setFrameRate(60);

	
	// connect to mosquitto.org test server using port 1883 (TCP transport layer)
	mMqtt = sitara::paho::MqttClient::make("test.mosquitto.org:1883", "Sitara Systems Test Client");
	// connect to mosquitto.org test server usign port 8080 (and websocket transport layer)
	//mMqtt = sitara::paho::MqttClient::make("ws://test.mosquitto.org:8080", "Sitara Systems Test Client");

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

void MqttClientExampleApp::mouseDown( MouseEvent event ) {
}

void MqttClientExampleApp::update() {
	if (ci::app::getElapsedFrames() % 300 == 0) {
		mMqtt->publish("sitara-systems", std::to_string(ci::app::getElapsedFrames()));
	}
}

void MqttClientExampleApp::draw() {
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( MqttClientExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {settings->setConsoleWindowEnabled(); })
