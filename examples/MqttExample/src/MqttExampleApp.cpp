#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Mqtt.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace sitara::mqtt;

class MqttExampleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::shared_ptr<sitara::mqtt::MqttClient> mClient;
};

void MqttExampleApp::setup() {
	mClient = sitara::mqtt::MqttClient::make("Sitara-Mqtt Client", "test.mosquitto.org", 1883);
	mClient->connect();
	mClient->subscribe("sitara-mqtt/testMessages");
	mClient->addOnReceiveFn([=](std::shared_ptr<MqttMessage> msg) {
		std::printf("Received message from server!\n Message Payload: %s\n Message Topic: %s\n Message QoS: %d\n",
			msg->getPayload().c_str(), msg->getTopic().c_str(), msg->getQualityOfService());
	});

}

void MqttExampleApp::mouseDown( MouseEvent event )
{
}

void MqttExampleApp::update() {
	if ((ci::app::getElapsedFrames() % 60) == 0 && mClient->isConnected()) {
		std::printf("Sending test message...\n");
		mClient->publish("sitara-mqtt/testMessages", "test test test");
	}
}

void MqttExampleApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
}

CINDER_APP( MqttExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {settings->setConsoleWindowEnabled(); })
