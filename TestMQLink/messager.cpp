#include "messager.h"
#include <QDateTime>
#include <QDebug>

Messager::Messager(QObject *parent)
    : QObject{parent}
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &Messager::onTimeout_slot);

    mqlink = new MQLink();
    qDebug() << "Device id:" << mqlink->getDeviceId();
    qDebug() << "Group id:" << mqlink->getGroupId();

    mqlink->zmq.setCallback([this](const std::string& msg) {
        this->myCallback(msg);
    });

    mqlink->mqtt.connect("localhost");

    mqlink->mqtt.setMsgCallback([this](const std::string& topic, const std::string& msg) {
        this->myCallback1(topic, msg);
    });

    mqlink->zmq.connect("localhost");

    // mqlink->zmq.sendFileContent("C:/Users/User/Downloads/OIP.jpg", true);
    // mqlink->zmq.sendFileContent("C:/Users/User/Desktop/sss1.txt", false);
    m_timer->start(500);
}

Messager::~Messager()
{
    if (mqlink != nullptr) {
        delete mqlink;
    }
}

void Messager::myCallback(const std::string &message)
{
    QString str = QString::fromStdString(message);
    // qDebug() << "CB:" << str;
}

void Messager::myCallback1(const std::string &topic, const std::string &message)
{
    // qDebug() << "CB1:" << topic << message;
}

void Messager::onTimeout_slot()
{
    static int a = 0;

    QDateTime startTime = QDateTime::currentDateTime();
    QString str = QString("%1 Test ZMQ[%2]").arg(startTime.toString("hh:mm:ss.zzz")).arg(a++);
    // mqlink->zmq.sendMessage(str.toStdString());
    qDebug().noquote() << "Start Sending Image:" << startTime.toString("hh:mm:ss.zzz");
    mqlink->zmq.sendFileContent("../OIP.jpg", true);
    // mqlink->mqtt.sendFileContent("test/topic", "../OIP.jpg");
    QDateTime endTime = QDateTime::currentDateTime();
    qint64 elapsed = startTime.msecsTo(endTime);

    qDebug() << "File sent in" << elapsed << "ms";

    // mqlink->mqtt.publish("test/topic", str.toStdString());
}
