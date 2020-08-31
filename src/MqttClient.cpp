#include "MqttClient.h"

using namespace sitara::paho;

MqttClient::~MqttClient() {
}

std::shared_ptr<MqttClient> MqttClient::make(std::string uri, std::string client, bool cleanSession) {
	std::shared_ptr<MqttClient> mqttClient(new MqttClient(uri, client, cleanSession));
	return mqttClient;
}

mqtt::async_client_ptr MqttClient::getClient() {
	return mClient;
}

std::string MqttClient::getClientName() {
	return mClientName;
}

std::string sitara::paho::MqttClient::getUriName() {
	return mUri;
}

void MqttClient::setConnectionOptions(const mqtt::connect_options& connOpts) {
	mConnectionOptions = std::make_shared<mqtt::connect_options>(connOpts);
}

void MqttClient::setReconnectionAttempts(int reconnectAttempts) {
}

void MqttClient::setSslOptions(const mqtt::ssl_options& sslOpts) {
	mConnectionOptions->set_ssl(sslOpts);
}

void MqttClient::setUsernamePassword(std::string username, std::string password) {
	mConnectionOptions->set_user_name(username);
	mConnectionOptions->set_password(password);
}

void MqttClient::setConnectedHandler(std::function<void(const std::string&)> cb) {
	mClient->set_connected_handler(cb);
}

void MqttClient::setConnectionLostHandler(std::function<void(const std::string&)> cb) {
	mClient->set_connection_lost_handler(cb);
}

void MqttClient::setDisconnectedHandler(std::function<void(const mqtt::properties&, mqtt::ReasonCode)> cb) {
	mClient->set_disconnected_handler(cb);
}

void MqttClient::setOnReceiveHandler(std::function<void(mqtt::const_message_ptr)> cb) {
	mClient->set_message_callback(cb);
}

void MqttClient::start() {
	try {
		std::cout << "Connecting to the MQTT server..." << std::flush;
		mClient->connect(*mConnectionOptions);
	}
	catch (const mqtt::exception&) {
		std::cerr << "\nERROR: Unable to connect to MQTT server: '"
			<< mClient->get_server_uri() << "'" << std::endl;
	}
}

void MqttClient::stop() {
	try {
		std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
		mClient->disconnect()->wait();
	}
	catch (const mqtt::exception& exc) {
		std::cerr << exc.what() << std::endl;
	}
}

MqttClient::MqttClient(std::string uri, std::string client, bool cleanSession) {
	mClient = std::make_shared<mqtt::async_client>(uri, client);
	mClientName = client;
	mUri = uri;

	mConnectionOptions = std::make_shared<mqtt::connect_options>();
	mConnectionOptions->set_automatic_reconnect(true);
	mConnectionOptions->set_keep_alive_interval(20);
	mConnectionOptions->set_clean_session(cleanSession);

	/*
	Setup default handlers -- set up your application-specific ones if you prefer!
	*/

	setConnectedHandler([&](const std::string& cause) {
		std::cout << "\nConnection success!" << std::endl;
	});

	setConnectionLostHandler([&](const std::string& cause) {
		std::cout << "\nConnection lost!" << std::endl;
		if (!cause.empty())
			std::cout << "\tCause: " << cause << std::endl;

		//std::cout << "Reconnecting..." << std::endl;
		//nretry_ = 0;
		//reconnect();
	});

	setDisconnectedHandler([&](const mqtt::properties& props, mqtt::ReasonCode rc) {
		std::cout << "\nDisconnected client." << std::endl;
		std::cout << "\tCause: " << mqtt::exception::reason_code_str(rc) << std::endl;

	});

	setOnReceiveHandler([&](mqtt::const_message_ptr msg) {
		std::cout << "Message arrived!" << std::endl;
		std::cout << "\tTopic: '" << msg->get_topic() << "'" << std::endl;
		std::cout << "\tPayload: '" << msg->to_string() << "'\n" << std::endl;
	});
}
