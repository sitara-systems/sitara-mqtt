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

    mMqtt = sitara::paho::MqttClient::make("test.mosquitto.org", "Sitara Test Client");

	mMqtt->setConnectedHandler([&](const std::string& cause) {
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
		mMqtt->getClient()->publish("sitara-systems", std::to_string(ci::app::getElapsedFrames()));
	}
}

void MqttClientExampleApp::draw() {
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( MqttClientExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {settings->setConsoleWindowEnabled(); })
