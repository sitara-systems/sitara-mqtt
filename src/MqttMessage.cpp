#include "MqttMessage.h"

using namespace sitara::mqtt;

MqttMessage::MqttMessage() {

}

MqttMessage::MqttMessage(const struct mosquitto_message* message) {
	mId = message->mid;
	mTopic = message->topic;
	mPayload = static_cast<std::string>((const char*)message->payload);
	mPayloadLength = message->payloadlen;
	mQualityOfService = message->qos;
	mIsRetained = message->retain;
}

std::string MqttMessage::getPayload() {
	return mPayload;
}

int MqttMessage::getPayloadSize() {
	return mPayloadLength;
}

std::string MqttMessage::getTopic() {
	return mTopic;
}
int MqttMessage::getQualityOfService() {
	return mQualityOfService;
}
bool MqttMessage::isRetained() {
	return mIsRetained;
}
