#include "zmq_messager.h"
#include "f_common.h"
#include "settings/f_settings.h"
#include <QDebug>

ZmqMessager *ZmqMessager::GetInstance()
{
#if 0
    static ZmqMessager instance;
    return &instance;
#else
    static ZmqMessager *instance = nullptr;
    if (instance == nullptr) {
        instance = new ZmqMessager();
    }
    return instance;
#endif
}

ZmqMessager::ZmqMessager(QObject *parent)
    : QObject{parent}
{
    m_id = FSettings::GetInstance()->deviceId();

    client = new ZmqClient(this);
    server = new ZmqServer(this);
    server->setDownloadPath(FCommon::getPath("downloads"));

    autoTestTimer = new QTimer(this);
    autoTestTimer->setInterval(1000);
    connect(autoTestTimer, &QTimer::timeout, this, &ZmqMessager::onAutoTestTimeout_slot);

    QJsonObject obj = FCommon::getConfigFileValue("zmq").toObject();
    if (!obj.isEmpty()) {
        setAutoTest(obj);
    }
}

ZmqMessager::~ZmqMessager()
{
    qDebug() << __FUNCTION__;
}

void ZmqMessager::setAutoTest(QJsonObject obj)
{
    QJsonObject clientObj = obj.value("client").toObject();
    QJsonObject serverObj = obj.value("server").toObject();

    qDebug() << __FUNCTION__ << clientObj << serverObj;

    /* Zmq Client*/
    bool clientEn = clientObj.value("enable").toBool();
    if (clientEn) {
        /* Connetion */
        QString address = clientObj.value("address").toString();
        int port = clientObj.value("port").toInt();
        client->connectServer(QString("tcp://%1:%2").arg(address).arg(port));

        /* Auto Test */
        QJsonObject autotestObj = clientObj.value("autotest").toObject();
        bool autoTestEn = autotestObj.value("enable").toBool();
        if (autoTestEn) {
            int interval = autotestObj.value("interval").toInt();
            m_autoTest = true;
            zmqMessageType = autotestObj.value("type").toString();
            autoTestTimer->start(interval);

            zmqATMessageArr = autotestObj.value("message").toArray();
            zmqATFilePathArr = autotestObj.value("filepath").toArray();
            qDebug() << "Zmq Start Auto Test.";
        }
    }

    /* Zmq Server*/
    bool serverEn = serverObj.value("enable").toBool();
    if (serverEn == true) {
        int port = serverObj.value("port").toInt();

        server->bindAddress(QString("tcp://*:%1").arg(port));
    }
}

void ZmqMessager::onAutoTestTimeout_slot()
{
    static int n = 1;
    QString current_time = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

    if (zmqMessageType == "text") {
        if (m_counter >= zmqATMessageArr.count()) {
            m_counter = 0;
        }
        QString content = zmqATMessageArr.at(m_counter).toString();
        client->sendMessage(QString("[%1](%2)%3: %4").arg(current_time, m_id).arg(n++).arg(content));
        m_counter++;
    } else {
        if (m_counter >= zmqATFilePathArr.count()) {
            m_counter = 0;
        }

        QString filepath = zmqATFilePathArr.at(m_counter).toString();
        client->sendFileContent(filepath);
        m_counter++;
    }
}
