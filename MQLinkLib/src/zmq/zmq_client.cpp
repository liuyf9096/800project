#include "zmq_client.h"
#include <zmq.h>
#include <QDebug>

ZmqClient::ZmqClient(QObject *parent)
    : QObject{parent}
    , m_context(1)
    , m_socket(m_context, zmq::socket_type::dealer)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(10);
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
    m_socket.connect(address.toLocal8Bit().constData());
    m_timer->start();
    m_serverAddress = address;
    qDebug() << "Connecting to server at" << m_serverAddress << ".OK";

    // if (zmq_connect(m_socket, ) == 0) {
    //     m_timer->start();
    //     m_serverAddress = address;
    //     qDebug() << "Connected to server at" << m_serverAddress << ".OK";
    //     return true;
    // } else {
    //     qDebug() << "Failed to connect to server: " << address;
    // }
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
    zmq::message_t request(message.toUtf8().constData(), message.size());
    m_socket.send(request, zmq::send_flags::none);

    qDebug() << "[Zmq Client]>>" << message;
}

void ZmqClient::sendBinaryFile(QByteArray header, QByteArray data)
{
    zmq_msg_t headerMsg;
    zmq_msg_init_size(&headerMsg, header.size());
    memcpy(zmq_msg_data(&headerMsg), header.constData(), header.size());
    zmq_msg_send(&headerMsg, m_socket, ZMQ_SNDMORE);
    zmq_msg_close(&headerMsg);

    zmq_msg_t dataMsg;
    zmq_msg_init_size(&dataMsg, data.size());
    memcpy(zmq_msg_data(&dataMsg), data.constData(), data.size());
    zmq_msg_send(&dataMsg, m_socket, 0);
    zmq_msg_close(&dataMsg);
}

void ZmqClient::sendData(QByteArray data)
{
    zmq_send(m_socket, data.constData(), data.size(), 0);
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
