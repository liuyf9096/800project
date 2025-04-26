#include "zmq_client.h"
#include <zmq.h>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

ZmqClient::ZmqClient(QObject *parent)
    : QObject{parent}
    , m_context(1)
    , m_socket(m_context, zmq::socket_type::dealer)
    , m_mode(ZMQ_C_DEALER)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(50);
    connect(m_timer, &QTimer::timeout, this, &ZmqClient::onTimeout_slot);
}

ZmqClient::~ZmqClient()
{
    m_socket.close();
    qDebug() << __FUNCTION__;
}

void ZmqClient::setClientMode(ZmqClientMode mode)
{
    m_socket.close();

    m_mode = mode;

    if (m_mode == ZMQ_C_REQ) {
        m_socket = zmq::socket_t(m_context, zmq::socket_type::req);
        qDebug() << "Client Set ZMQ_REQ mode.";
    } else if (m_mode == ZMQ_C_SUB) {
        m_socket = zmq::socket_t(m_context, zmq::socket_type::sub);
        qDebug() << "Client Set ZMQ_SUB mode.";
    } else if (m_mode == ZMQ_C_PULL) {
        m_socket = zmq::socket_t(m_context, zmq::socket_type::pull);
        qDebug() << "Client Set ZMQ_PULL mode.";
    } else if (m_mode == ZMQ_C_DEALER) {
        m_socket = zmq::socket_t(m_context, zmq::socket_type::dealer);
        qDebug() << "Client Set ZMQ_DEALER mode.";
    } else {
        m_mode = ZMQ_C_Undefine;
        qDebug() << "Error";
    }
}

void ZmqClient::setLoopTimer(int msec)
{
    m_timer->setInterval(msec);
}

void ZmqClient::connectServer(QString address)
{
    m_socket.connect(address.toLocal8Bit().constData());
    m_timer->start();
    m_serverAddress = address;
    qDebug() << "Connecting to server at" << m_serverAddress << ".OK";
}

void ZmqClient::disconnect()
{
    m_timer->stop();
    m_socket.disconnect(m_serverAddress.toLocal8Bit().constData());
}

void ZmqClient::sendMessage(QString message)
{
    QByteArray header;
    QDataStream stream(&header, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << quint32(0xB1B2F1F3);
    QByteArray textData = message.toUtf8();
    stream << quint16(textData.size());
    stream.writeRawData(textData.constData(), textData.size());

    zmq::message_t headerMsg(header.size());
    memcpy(headerMsg.data(), header.constData(), header.size());
    m_socket.send(headerMsg, zmq::send_flags::sndmore);

    zmq::message_t textMsg(textData.size());
    memcpy(textMsg.data(), textData.constData(), textData.size());
    m_socket.send(textMsg, zmq::send_flags::none);

#if 0
    zmq::message_t request(message.toUtf8().constData(), message.size());
    m_socket.send(request, zmq::send_flags::none);
#endif

    qDebug() << "<<[Zmq Client]" << message;
}

bool ZmqClient::sendFileContent(QString filePath)
{
    QByteArray header;
    QDataStream stream(&header, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << filePath;
        return false;
    }

    QByteArray fileData = file.readAll();
    QFileInfo info(file);
    QByteArray fileName = info.fileName().toUtf8();

    stream << quint32(0xB1B2F1F2);
    stream << quint16(fileName.size());
    stream.writeRawData(fileName.constData(), fileName.size());
    stream << quint32(fileData.size());

    zmq::message_t headerMsg(header.size());
    memcpy(headerMsg.data(), header.constData(), header.size());
    m_socket.send(headerMsg, zmq::send_flags::sndmore);

    zmq::message_t dataMsg(fileData.size());
    memcpy(dataMsg.data(), fileData.constData(), fileData.size());
    m_socket.send(dataMsg, zmq::send_flags::none);

    qDebug() << "<<[Zmq Client]"
             << QString("[File]%1 size:%2").arg(filePath).arg(fileData.size());
    // qDebug() << header.toHex();

#if 0
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << filePath;
        return false;
    }

    QByteArray fileData = file.readAll();

    zmq::message_t msg(fileData.size());
    memcpy(msg.data(), fileData.constData(), fileData.size());
    m_socket.send(msg, zmq::send_flags::none);
#endif
    return true;
}

bool ZmqClient::sendBinaryFile(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file: " << filePath;
        return false;
    }

    QByteArray fileData = file.readAll();

    QFileInfo info(file);
    QByteArray fileName = info.fileName().toUtf8();

    QByteArray header;
    QDataStream stream(&header, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);

    stream << quint32(0xB1B2F1F2);
    stream << quint16(fileName.size());
    stream.writeRawData(fileName.constData(), fileName.size());
    stream << quint32(fileData.size());

    zmq::message_t headerMsg(header.size());
    memcpy(headerMsg.data(), header.constData(), header.size());
    m_socket.send(headerMsg, zmq::send_flags::sndmore);

    zmq::message_t dataMsg(fileData.size());
    memcpy(dataMsg.data(), fileData.constData(), fileData.size());
    m_socket.send(dataMsg, zmq::send_flags::none);

    qDebug() << "<<[Zmq Client]"
             << QString("[File]%1 size:%2").arg(filePath).arg(fileData.size());
    // qDebug() << header.toHex();
    return true;
}

void ZmqClient::receiveloop()
{
    char buffer[256];
    int recv_size = zmq_recv(m_socket, buffer, sizeof(buffer) - 1, ZMQ_DONTWAIT);
    if (recv_size > 0) {
        buffer[recv_size] = '\0';
        qDebug() << ">>[Client]" << buffer;
    }
}

void ZmqClient::receiveRouterLoop()
{
    char identity[256];
    int rc = zmq_recv(m_socket, identity, sizeof(identity) - 1, ZMQ_DONTWAIT);
    if (rc < 0) {
        // qDebug() << "Failed to receive identity frame! Error:" << zmq_strerror(errno);
        return;
    }
    identity[rc] = '\0';

    int more;
    size_t more_size = sizeof(more);
    zmq_getsockopt(m_socket, ZMQ_RCVMORE, &more, &more_size);

    if (more) {
        char buffer[256];
        rc = zmq_recv(m_socket, buffer, sizeof(buffer) - 1, ZMQ_DONTWAIT);
        if (rc < 0) {
            qDebug() << "Failed to receive data frame! Error:" << zmq_strerror(errno);
            return;
        }
        buffer[rc] = '\0';
        QString message = QString::fromUtf8(buffer);

        qDebug() << "Received identity frame:" << QByteArray(identity, rc).toHex();
        qDebug() << "Received data frame:" << message;
    } else {
        qDebug() << "No data frame received!";
    }
}

void ZmqClient::onTimeout_slot()
{
    if (m_socket == nullptr) {
        return;
    }

    if (m_mode == ZMQ_C_DEALER) {
        receiveloop();
        // receiveRouterLoop();
    } else {
        receiveloop();
    }
}
