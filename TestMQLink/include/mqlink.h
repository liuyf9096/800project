#ifndef MQLINK_H
#define MQLINK_H

#include <QtCore/qglobal.h>

#if defined(MQLINKLIB_LIBRARY)
#define MQLINKLIB_EXPORT Q_DECL_EXPORT
#else
#define MQLINKLIB_EXPORT Q_DECL_IMPORT
#endif

#include <memory>
#include <string>
#include <functional>

#ifdef __cplusplus
extern "C" {
#endif

class ZmqMessagerPrivate;
class MQLINKLIB_EXPORT ZmqMessager
{
public:
    using ZmqMessageCallback = std::function<void(const std::string&)>;

    ZmqMessager();
    ~ZmqMessager();

    void connect(const std::string& address, int port = 5555);
    void disconnect();

    void sendMessage(const std::string& message);
    bool sendFileContent(const std::string& fileName, bool isBinary = false);
    void setReceiveFilePath(const std::string& filePath);

    /* receive message callback */
    void setCallback(ZmqMessageCallback callback);

private:
    std::unique_ptr<ZmqMessagerPrivate> d_ptr;
};

class MqttMessagerPrivate;
class MQLINKLIB_EXPORT MqttMessager
{
public:
    using MqttMessageCallback = std::function<void(const std::string& topic, const std::string& message)>;
    using MqttFileCallback = std::function<void(const std::string& topic, const std::string& data)>;
    MqttMessager();
    ~MqttMessager();

    void connect(const std::string& address, int port = 1883);
    void disconnect();

    void publish(const std::string& topic, const std::string& message);

    bool sendFileContent(const std::string& topic, const std::string& fileName);
    void setReceiveFilePath(const std::string& filePath);

    /* receive message callback */
    void setMsgCallback(MqttMessageCallback callback);
    void setFileCallback(MqttFileCallback callback);

private:
    std::unique_ptr<MqttMessagerPrivate> d_ptr;
};

class MQLinkPrivate;
class MQLINKLIB_EXPORT MQLink
{
public:
    MQLink();
    ~MQLink();

    std::string getDeviceId();
    std::string getGroupId();

    ZmqMessager zmq;
    MqttMessager mqtt;

private:
    std::unique_ptr<MQLinkPrivate> d_ptr;
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // MQLINK_H
