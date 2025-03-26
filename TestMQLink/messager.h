#ifndef MESSAGER_H
#define MESSAGER_H

#include <QObject>
#include <QTimer>
#include "include/mqlink.h"

class Messager : public QObject
{
    Q_OBJECT
public:
    explicit Messager(QObject *parent = nullptr);
    ~Messager();

    MQLink *mqlink;

    void myCallback(const std::string& message);
    void myCallback1(const std::string& topic, const std::string& message);

private:
    QTimer *m_timer;

private slots:
    void onTimeout_slot();
};

#endif // MESSAGER_H
