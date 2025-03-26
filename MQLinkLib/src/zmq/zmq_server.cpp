#include "zmq_server.h"
#include <zmq.h>
#include <QDebug>

ZmqServer::ZmqServer(QObject *parent)
    : QObject{parent}
{
    m_timer = new QTimer(this);
    m_timer->setInterval(10);
    connect(m_timer, &QTimer::timeout, this, &ZmqServer::onTimeout_slot);

    m_context = zmq_ctx_new();
    if (!m_context) {
        qDebug() << "Failed to create ZeroMQ context!";
    }

    setClientMode(ZMQ_S_ROUTER);
}

ZmqServer::~ZmqServer()
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

void ZmqServer::setClientMode(ZmqServerMode mode)
{
    if (m_socket != nullptr) {
        zmq_close(m_socket);
        m_socket = nullptr;
    }

    m_mode = mode;

    if (m_mode == ZMQ_S_REP) {
        m_socket = zmq_socket(m_context, ZMQ_REP);
        qDebug() << "Server Set ZMQ_REP mode.";
    } else if (m_mode == ZMQ_S_PUB) {
        m_socket = zmq_socket(m_context, ZMQ_PUB);
        qDebug() << "Server Set ZMQ_PUB mode.";
    } else if (m_mode == ZMQ_S_PUSH) {
        m_socket = zmq_socket(m_context, ZMQ_PUSH);
        qDebug() << "Server Set ZMQ_PUSH mode.";
    } else if (m_mode == ZMQ_S_ROUTER) {
        m_socket = zmq_socket(m_context, ZMQ_ROUTER);
        qDebug() << "Server Set ZMQ_ROUTER mode.";
    } else {
        m_mode = ZMQ_S_Undefine;
        qDebug() << "Error";
    }

    if (m_socket == nullptr) {
        qDebug() << "Failed to create ZeroMQ socket!";
    }
}

void ZmqServer::bindAddress(QString address)
{
    if (zmq_bind(m_socket, address.toLocal8Bit().constData()) == 0) {
        m_timer->start();
#if 0
        int fd;
        size_t fd_size = sizeof(fd);
        zmq_getsockopt(responder, ZMQ_FD, &fd, &fd_size);
        m_notifier->setSocket(fd);
#endif
        qDebug() << "Server is listening on" << address;
    } else {
        qDebug() << "Failed to bind to address: " << address;
    }
}

void ZmqServer::sendMessage(QString clientId, QString message)
{
    if (m_socket == nullptr) {
        return;
    }

    if (m_mode == ZMQ_S_ROUTER && !m_identity.isEmpty()) {
        zmq_msg_t identity_frame;
        zmq_msg_init_size(&identity_frame, m_identity.size());
        memcpy(zmq_msg_data(&identity_frame), m_identity.constData(), m_identity.size());

        zmq_msg_t data_frame;
        QByteArray data = message.toUtf8();
        zmq_msg_init_size(&data_frame, data.size());
        memcpy(zmq_msg_data(&data_frame), data.constData(), data.size());

        int rc = zmq_msg_send(&identity_frame, m_socket, ZMQ_SNDMORE);
        if (rc < 0) {
            qDebug() << "Failed to send identity frame!";
            zmq_msg_close(&identity_frame);
            zmq_msg_close(&data_frame);
            return;
        }

        rc = zmq_msg_send(&data_frame, m_socket, 0);
        if (rc < 0) {
            qDebug() << "Failed to send data frame!";
            zmq_msg_close(&identity_frame);
            zmq_msg_close(&data_frame);
            return;
        }

        qDebug() << "[Zmq Server]>>" << message;

        zmq_msg_close(&identity_frame);
        zmq_msg_close(&data_frame);
    } else {
        if (zmq_send(m_socket, message.toLocal8Bit().constData(), message.size(), 0) < 0) {
            qDebug() << "Failed to send message!";
            return;
        }

        qDebug() << "[Zmq Server]>>" << message;
    }
}

void ZmqServer::handleReqRepLoop()
{
    char buffer[256];
    int recv_size = zmq_recv(m_socket, buffer, sizeof(buffer) - 1, ZMQ_DONTWAIT);
    if (recv_size < 0) {
        // qDebug() << "Failed to receive message!";
        return;
    }

    buffer[recv_size] = '\0';
    QString str = QString::fromUtf8(buffer);

    qDebug() << ">>[Zmq Server]" << str;
    emit onReceiveMessage_signal(str);

    const char* response = "..OK";
    if (zmq_send(m_socket, response, strlen(response), ZMQ_DONTWAIT) < 0) {
        qDebug() << "Failed to send message! 'OK'";
        return;
    }

    qDebug() << "[Zmq Server]>>" << response;
}

void ZmqServer::handlePubSubLoop()
{

}

void ZmqServer::handlePushPullLoop()
{

}

void ZmqServer::handleRouterDealerLoop()
{
    zmq_msg_t identity_frame;
    zmq_msg_init(&identity_frame);
    int rc = zmq_msg_recv(&identity_frame, m_socket, ZMQ_DONTWAIT);
    if (rc < 0) {
        // qDebug() << "Failed to receive identity frame!";
        // zmq_msg_close(&identity_frame);
        return;
    }

    zmq_msg_t data_frame;
    zmq_msg_init(&data_frame);
    rc = zmq_msg_recv(&data_frame, m_socket, ZMQ_DONTWAIT);
    if (rc < 0) {
        qDebug() << "Failed to receive data frame!";
        zmq_msg_close(&identity_frame);
        zmq_msg_close(&data_frame);
        return;
    }

    QByteArray identity(static_cast<char*>(zmq_msg_data(&identity_frame)), zmq_msg_size(&identity_frame));
    QByteArray data(static_cast<char*>(zmq_msg_data(&data_frame)), zmq_msg_size(&data_frame));
    qDebug() << "receive data size:" << data.size();

    QString message = QString::fromUtf8(data);
    m_identity = identity;

    qDebug() << ">>[Zmq Server]" << message;
    emit onReceiveMessage_signal(message);

    zmq_msg_close(&identity_frame);
    zmq_msg_close(&data_frame);
}

void ZmqServer::onTimeout_slot()
{
    if (m_socket == nullptr) {
        return;
    }

    if (m_mode == ZMQ_S_REP) {
        handleReqRepLoop();
    } else if (m_mode == ZMQ_S_PUB) {
        handlePubSubLoop();
    } else if (m_mode == ZMQ_S_PUSH) {
        handlePushPullLoop();
    } else if (m_mode == ZMQ_S_ROUTER) {
        handleRouterDealerLoop();
    }
}
