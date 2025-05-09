#include "zmq_messager.h"
#include "f_common.h"
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
    client = new ZmqClient(this);
    server = new ZmqServer(this);
    server->setDownloadPath(FCommon::getDownloadsPath());

    autoTestTimer = new QTimer(this);
    autoTestTimer->setInterval(1000);
    connect(autoTestTimer, &QTimer::timeout, this, &ZmqMessager::onAutoTestTimeout_slot);
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
        QString ip = clientObj.value("ip").toString();
        int port = clientObj.value("port").toInt();

        client->connectServer(QString("tcp://%1:%2").arg(ip).arg(port));
        bool autoTest = clientObj.value("autoTest").toBool();
        if (autoTest) {
            int interval = clientObj.value("autoTest_interval").toInt();
            m_autoTest = true;
            testType = clientObj.value("testType").toString();
            autoTestTimer->start(interval);

            zmqATContent = clientObj.value("autoTest_content").toString();
            zmqFileNum = clientObj.value("send_file_number").toInt();
            zmqFileName_1 = clientObj.value("send_filepath_1").toString();
            zmqFileName_2 = clientObj.value("send_filepath_2").toString();
            zmqFileName_3 = clientObj.value("send_filepath_3").toString();
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

    if (testType == "text") {
        QString current_time = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
        client->sendMessage(QString("[%1](%2)%3: %4").arg(current_time, m_id).arg(n++).arg(zmqATContent));
    } else if (zmqFileNum > 0) {
        if (n > zmqFileNum) {
            n = 1;
        }
        if (n == 1) {
            client->sendFileContent(zmqFileName_1);
        } else if (n == 2) {
            client->sendFileContent(zmqFileName_2);
        } else if (n == 3) {
            client->sendFileContent(zmqFileName_3);
        }
        n++;
    }
}
