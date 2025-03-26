#include "messager.h"
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

    mqlink->zmq.connect("localhost");


    mqlink->zmq.sendFileContent("C:/Users/User/Downloads/OIP.jpg", true);
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
    qDebug() << "CB:" << str;
}

void Messager::onTimeout_slot()
{
    static int a = 0;
    QString str = QString::number(a++);
    mqlink->zmq.sendMessage(str.toStdString());
}
