#include "zmq_client.h"
#include <zmq.h>
#include <QDebug>

ZmqClient::ZmqClient(QObject *parent)
    : QObject{parent}
{
    m_timer = new QTimer(this);
    m_timer->setInterval(10);
    connect(m_timer, &QTimer::timeout, this, &ZmqClient::onTimeout_slot);

    m_context = zmq_ctx_new();
    if (!m_context) {
        qFatal() << "Failed to create ZeroMQ context!";
    }

    setMode(ZMQ_C_DEALER);
}

ZmqClient::~ZmqClient()
{
    if (m_socket != nullptr) {
        zmq_close(m_socket);
        m_socket = nullptr;
    }
    if (m_context != nullptr) {
        zmq_ctx_destroy(m_context);
        m_context = nullptr;
    }
    qDebug() << __FUNCTION__;
}

void ZmqClient::setMode(ZmqClientMode mode)
{
    if (m_socket != nullptr) {
        zmq_close(m_socket);
        m_socket = nullptr;
    }

    m_mode = mode;

    if (m_mode == ZMQ_C_REQ) {
        m_socket = zmq_socket(m_context, ZMQ_REQ);
        qDebug() << "Client Set ZMQ_REQ mode.";
    } else if (m_mode == ZMQ_C_SUB) {
        m_socket = zmq_socket(m_context, ZMQ_SUB);
        qDebug() << "Client Set ZMQ_SUB mode.";
    } else if (m_mode == ZMQ_C_PULL) {
        m_socket = zmq_socket(m_context, ZMQ_PULL);
        qDebug() << "Client Set ZMQ_PULL mode.";
    } else if (m_mode == ZMQ_C_DEALER) {
        m_socket = zmq_socket(m_context, ZMQ_DEALER);
        qDebug() << "Client Set ZMQ_DEALER mode.";
    } else {
        m_mode = ZMQ_C_Undefine;
        qDebug() << "Error";
    }

    if (m_socket == nullptr) {
        qDebug() << "Failed to create ZeroMQ socket!";
    }
}

void ZmqClient::setLoopTimer(int msec)
{
    m_timer->setInterval(msec);
}

void ZmqClient::connectServer(QString address)
{
    if (m_socket == nullptr) {
        return;
    }

    if (zmq_connect(m_socket, address.toLocal8Bit().constData()) == 0) {
        m_timer->start();
        m_serverAddress = address;
        qDebug() << "Connected to server at" << m_serverAddress << ".OK";
    } else {
        qDebug() << "Failed to connect to server: " << address;
    }
}

void ZmqClient::disconnect()
{
    m_timer->stop();
    if (zmq_disconnect(m_socket, m_serverAddress.toLocal8Bit().constData())) {
        qDebug() << "Disconnected." << m_serverAddress;
    }
}

void ZmqClient::sendMessage(QString message)
{
    if (m_socket == nullptr) {
        return;
    }

    if (zmq_send(m_socket, message.toLocal8Bit().constData(), message.size(), 0) < 0) {
        qDebug() << "Failed to send message!";
        return;
    }
    qDebug() << "[Client]>>" << message;
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
