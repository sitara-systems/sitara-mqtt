#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "ofxMqtt.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class exampleMqttClientApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	std::shared_ptr<midnight::mqtt::MqttClient> mClient;
};

void exampleMqttClientApp::setup()
{
	mClient = midnight::mqtt::MqttClient::make("ofxMqtt Client", "10.10.10.3", 1883);
	mClient->connect();
	mClient->subscribe("ofxMqtt/testMessages");
	mClient->addOnReceiveFn([=](std::shared_ptr<midnight::mqtt::MqttMessage> msg) {
		std::printf("Received message from server!\n Message Payload: %s\n Message Topic: %s\n Message QoS: %d\n", 
			msg->getPayload().c_str(), msg->getTopic().c_str(), msg->getQualityOfService());
	});
}

void exampleMqttClientApp::mouseDown( MouseEvent event )
{
}

void exampleMqttClientApp::update() {
	if ((ci::app::getElapsedFrames() % 60) == 0 && mClient->isConnected()) {
		std::printf("Sending test message...\n");
		mClient->publish("ofxMqtt/testMessages", "test test test");
	}
}

void exampleMqttClientApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
}

CINDER_APP( exampleMqttClientApp, RendererGl, [=](cinder::app::App::Settings* settings) {settings->setConsoleWindowEnabled(); })
