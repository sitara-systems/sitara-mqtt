#pragma once

#include <mqtt/async_client.h>

namespace sitara {
    namespace paho {
        class MqttClient {
        public:
            ~MqttClient();
            static std::shared_ptr<MqttClient> make(std::string uri, std::string client, bool cleanSession = true);
            mqtt::async_client_ptr getClient();
            std::string getClientName();
            std::string getUriName();
            void setConnectionOptions(const mqtt::connect_options& connOpts);
            void setReconnectionAttempts(int reconnectAttempts);
            void setSslOptions(const mqtt::ssl_options& sslOpts);
            void setUsernamePassword(std::string username, std::string password);
            void setConnectedHandler(std::function<void(const std::string&)> cb);
            void setConnectionLostHandler(std::function<void(const std::string&)> cb);
            void setDisconnectedHandler(std::function<void(const mqtt::properties&, mqtt::ReasonCode)> cb);
            void setOnReceiveHandler(std::function<void(mqtt::const_message_ptr)> cb);
            void start();
            void stop();
            void publish(std::string topic, std::string payload);
        protected:
            MqttClient(std::string uri, std::string client, bool cleanSession = true);
            mqtt::async_client_ptr mClient;
            mqtt::connect_options_ptr mConnectionOptions;
            std::string mUri;
            std::string mClientName;
        };
    }
}
