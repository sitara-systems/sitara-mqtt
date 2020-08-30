#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "mqtt/async_client.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const std::string SERVER_ADDRESS("test.mosquitto.org");
const std::string CLIENT_ID("async_subcribe_cpp");
const std::string TOPIC("hello");
const int	QOS = 1;
const int	N_RETRY_ATTEMPTS = 5;

class action_listener : public virtual mqtt::iaction_listener
{
    std::string name_;

    void on_failure(const mqtt::token& tok) override {
        std::cout << name_ << " failure";
        if (tok.get_message_id() != 0)
            std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
        std::cout << std::endl;
    }

    void on_success(const mqtt::token& tok) override {
        std::cout << name_ << " success";
        if (tok.get_message_id() != 0)
            std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
        auto top = tok.get_topics();
        if (top && !top->empty())
            std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
        std::cout << std::endl;
    }

public:
    action_listener(const std::string& name) : name_(name) {}
};

class callback : public virtual mqtt::callback,
	public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry_;
	// The MQTT client
	mqtt::async_client& cli_;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts_;
	// An action listener to display the result of actions.
	action_listener subListener_;

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void reconnect() {
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		try {
			cli_.connect(connOpts_, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
			std::cerr << "Error: " << exc.what() << std::endl;
			exit(1);
		}
	}

	// Re-connection failure
	void on_failure(const mqtt::token& tok) override {
		std::cout << "Connection attempt failed" << std::endl;
		if (++nretry_ > N_RETRY_ATTEMPTS)
			exit(1);
		reconnect();
	}

	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void on_success(const mqtt::token& tok) override {}

	// (Re)connection success
	void connected(const std::string& cause) override {
		std::cout << "\nConnection success" << std::endl;
		std::cout << "\nSubscribing to topic '" << TOPIC << "'\n"
			<< "\tfor client " << CLIENT_ID
			<< " using QoS" << QOS << "\n"
			<< "\nPress Q<Enter> to quit\n" << std::endl;

		cli_.subscribe(TOPIC, QOS, nullptr, subListener_);
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const std::string& cause) override {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;

		std::cout << "Reconnecting..." << std::endl;
		nretry_ = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override {
		std::cout << "Message arrived" << std::endl;
		std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
		std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
	}

	void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
		: nretry_(0), cli_(cli), connOpts_(connOpts), subListener_("Subscription") {}
};

class MqttClientExampleApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void MqttClientExampleApp::setup() {
	mqtt::connect_options connOpts;
	connOpts.set_keep_alive_interval(20);
	connOpts.set_clean_session(true);

	mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

	callback cb(client, connOpts);
	client.set_callback(cb);

	// Start the connection.
	// When completed, the callback will subscribe to topic.

	try {
		std::cout << "Connecting to the MQTT server..." << std::flush;
		client.connect(connOpts, nullptr, cb);
	}
	catch (const mqtt::exception&) {
		std::cerr << "\nERROR: Unable to connect to MQTT server: '"
			<< SERVER_ADDRESS << "'" << std::endl;
	}

	// Just block till user tells us to quit.

	while (std::tolower(std::cin.get()) != 'q')
		;

	// Disconnect

	try {
		std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
		client.disconnect()->wait();
		std::cout << "OK" << std::endl;
	}
	catch (const mqtt::exception& exc) {
		std::cerr << exc.what() << std::endl;
	}
}

void MqttClientExampleApp::mouseDown( MouseEvent event )
{
}

void MqttClientExampleApp::update() {
}

void MqttClientExampleApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( MqttClientExampleApp, RendererGl, [=](cinder::app::App::Settings* settings) {settings->setConsoleWindowEnabled(); })
