#pragma once

#include "mosquittopp.h"
#include <string>

namespace sitara {
	namespace mqtt {
		class MqttMessage {
		public:
			MqttMessage();
			MqttMessage(const struct mosquitto_message* message);
			std::string getPayload();
			int getPayloadSize();
			std::string getTopic();
			int getQualityOfService();
			bool isRetained();
		protected:
			int mId;
			std::string mTopic;
			std::string mPayload;
			int mPayloadLength;
			int mQualityOfService;
			bool mIsRetained;
		};
	}
}