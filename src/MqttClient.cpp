#include "MqttClient.h"

using namespace sitara::mqtt;

MqttClient::MqttClient(std::string clientId, std::string hostname, int port, bool cleanSession) : mosqpp::mosquittopp(clientId.c_str(), cleanSession) {
  mosqpp::lib_init();
  mIsConnected = false;
  mIsAutoReconnect = false;
  mClientId = clientId;
  mHostname = hostname;
  mPort = port;
  mKeepAlive = 60;
  mRunUpdateThread = false;
}

MqttClient::~MqttClient() {
	mUpdateMutex.lock();
	if (mRunUpdateThread) {
		stop();
	}
	mosqpp::lib_cleanup();
	mUpdateMutex.unlock();
}

std::shared_ptr<MqttClient> MqttClient::make(std::string clientId, std::string hostname, int port, bool cleanSession) {
	std::shared_ptr<MqttClient> client = std::shared_ptr<MqttClient>(new MqttClient(clientId, hostname, port, cleanSession));
	return client;
}

void MqttClient::connect() {
	int result = checkForErrors(mosquittopp::connect(mHostname.c_str(), mPort, mKeepAlive));
	start();
};

void MqttClient::reconnect() {
	checkForErrors(mosquittopp::reconnect());
};

void MqttClient::disconnect() {
	stop();
	checkForErrors(mosquittopp::disconnect());
};

void MqttClient::publish(std::string topic, std::string payload, int qualityOfService, bool retain, int messageId) {
	checkForErrors(mosquittopp::publish(&messageId, topic.c_str(), payload.size(), payload.c_str(), qualityOfService, retain));
}

void MqttClient::subscribe(std::string sub, int qualityOfService, int messageId) {
	checkForErrors(mosquittopp::subscribe(&messageId, sub.c_str(), qualityOfService));
}

void MqttClient::unsubscribe(std::string sub, int messageId) {
	checkForErrors(mosquittopp::unsubscribe(&messageId, sub.c_str()));
}

void MqttClient::start() {
	mUpdateMutex.lock();
	mUpdateThread = std::thread(&MqttClient::updateClient, this);
	mRunUpdateThread = true;
	mUpdateMutex.unlock();
};

void MqttClient::stop() {
	mRunUpdateThread = false;
	if (mUpdateThread.joinable()) {
		mUpdateThread.join();
	}
};

std::string MqttClient::getClientId() {
	return mClientId;
};

std::string MqttClient::getHostname() {
	return mHostname;
};

int MqttClient::getPortNumber() {
	return mPort;
};

bool MqttClient::isConnected() {
	return mIsConnected;
};

void MqttClient::setKeepAlive(int keepAlive) {
	mUpdateMutex.lock();
	mKeepAlive = keepAlive;
	mUpdateMutex.unlock();
}

void MqttClient::setAutoReconnect(bool reconnect) {
	mUpdateMutex.lock();
	mIsAutoReconnect = reconnect;
	mUpdateMutex.unlock();
}

void MqttClient::addOnConnectFn(std::function<void()> callback) {
	mOnConnectFns.push_back(callback);
}

void MqttClient::addOnDisconnectFn(std::function<void()> callback) {
	mOnDisconnectFns.push_back(callback);
}

void MqttClient::addOnReceiveFn(std::function<void(std::shared_ptr<MqttMessage> message)> callback) {
	mOnReceiveFns.push_back(callback);
}

void MqttClient::addOnPublishFn(std::function<void()> callback) {
	mOnPublishFns.push_back(callback);
}

void MqttClient::addOnSubscribeFn(std::function<void(int messageId)> callback) {
	mOnSubscribeFns.push_back(callback);
}

void MqttClient::addOnUnsubscribeFn(std::function<void(int messageiId)> callback) {
	mOnUnsubscribeFns.push_back(callback);
}

int MqttClient::checkForErrors(int errorCode) {
	if (errorCode > 0) {
		std::printf("MqttClient | ERROR %d - %s\n", errorCode, mosqpp::strerror(errorCode));
		return 1;
	}
	else {
		return 0;
	}
}

void MqttClient::updateClient() {
	while (mRunUpdateThread) {
		mUpdateMutex.lock();
		int errorCode = mosquittopp::loop();
		if (errorCode != 0 && mIsAutoReconnect) {
			checkForErrors(errorCode);
			reconnect();
		}
		mUpdateMutex.unlock();
	}
}

void MqttClient::on_connect(int errorCode) {
	std::printf("MqttClient | Client Connected!\n");
	mIsConnected = true;
	for (auto callback : mOnConnectFns) {
		callback();
	}
}

void MqttClient::on_disconnect(int errorCode) {
	for (auto callback : mOnDisconnectFns) {
		callback();
	}
}

void MqttClient::on_message(const struct mosquitto_message* message) {
	std::shared_ptr<MqttMessage> msg = std::shared_ptr<MqttMessage>(new MqttMessage(message));

	for (auto callback : mOnReceiveFns) {
		callback(msg);
	}
}

void MqttClient::on_publish(int errorCode) {
	for (auto callback : mOnPublishFns) {
		callback();
	}
}

void MqttClient::on_subscribe(int messageId, int qos_count, const int* granted_qos) {
	for (auto callback : mOnSubscribeFns) {
		callback(messageId);
	}
}

void MqttClient::on_unsubscribe(int messageId) {
	for (auto callback : mOnUnsubscribeFns) {
		callback(messageId);
	}
}