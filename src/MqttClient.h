#pragma once

#include "mosquittopp.h"
#include "MqttMessage.h"
#include <memory>
#include <mutex>
#include <vector>
#include <thread>
#include <functional>

namespace sitara {
  namespace mqtt {
    class MqttClient : public mosqpp::mosquittopp {
    public:
      ~MqttClient();
      static std::shared_ptr<MqttClient> make(std::string clientId, std::string hostname, int port, bool cleanSession=true);
      void connect();
      void reconnect();
      void disconnect();
      void publish(std::string topic, std::string payload, int qualityOfService=0, bool retain=false, int messageId=NULL);
      void subscribe(std::string sub, int qualityOfService=0, int messageId=NULL);
      void unsubscribe(std::string sub, int messageId=NULL);
      void start();
      void stop();
      std::string getClientId();
      std::string getHostname();
      int getPortNumber();
      bool isConnected();
      void setKeepAlive(int keepAlive);
      void setAutoReconnect(bool reconnect);

	  void addOnConnectFn(std::function<void()> callback);
	  void addOnDisconnectFn(std::function<void()> callback);
	  void addOnReceiveFn(std::function<void(std::shared_ptr<MqttMessage> message)> callback);
	  void addOnPublishFn(std::function<void()> callback);
	  void addOnSubscribeFn(std::function<void(int messageId)> callback);
	  void addOnUnsubscribeFn(std::function<void(int messageiId)> callback);
	protected:
      MqttClient(std::string clientId, std::string hostname, int port, bool cleanSession=true);
	  int checkForErrors(int errorCode);
	  void updateClient();
	  void on_connect(int errorCode);
	  void on_disconnect(int errorCode);
	  void on_message(const struct mosquitto_message* message);
	  void on_publish(int errorCode);
	  void on_subscribe(int messageId, int qos_count, const int* granted_qos);
	  void on_unsubscribe(int messageId);
      std::string mClientId;
      std::string mHostname;
      int mPort;
      int mKeepAlive;
      bool mIsConnected;
      bool mIsAutoReconnect;
	  bool mRunUpdateThread;
	  std::thread mUpdateThread;
	  std::mutex mUpdateMutex;
	  std::vector<std::function<void()> > mOnConnectFns;
	  std::vector<std::function<void()> > mOnDisconnectFns;
	  std::vector<std::function<void(std::shared_ptr<MqttMessage> message)> > mOnReceiveFns;
	  std::vector<std::function<void()> > mOnPublishFns;
	  std::vector<std::function<void(int messageId)> > mOnSubscribeFns;
	  std::vector<std::function<void(int messageId)> > mOnUnsubscribeFns;
	};

  }
}
