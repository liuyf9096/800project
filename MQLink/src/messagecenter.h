#ifndef MESSAGECENTER_H
#define MESSAGECENTER_H

#include <QObject>
#include "mqtt/mqtt_messager.h"
#include "zmq/zmq_messager.h"

class QSocketNotifier;
class MessageCenter : public QObject
{
    Q_OBJECT
public:
    static MessageCenter *GetInstance();

private:
    explicit MessageCenter(QObject *parent = nullptr);
    Q_DISABLE_COPY(MessageCenter)
    ~MessageCenter();

    QSocketNotifier *m_notifier = nullptr;
    bool m_isConnected{false};

signals:

};

#endif // MESSAGECENTER_H
