#ifndef F_SETTINGS_H
#define F_SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDateTime>

class FSettings : public QObject
{
    Q_OBJECT
public:
    static FSettings *GetInstance();

    void setValue(const QString &key, const QVariant &value);
    QVariant getValue(const QString &key);

    QString deviceId();
    QString groupId();

    /* Zmq */
    int getZmqClientMode();
    int getZmqClientPort();

    int getZmqServerMode();
    int getZmqServerPort();

    /* Log */
    quint32 logindex();
    quint32 MaxLogFileCount();
    int logContentType();
    bool isLogServerEnable();

private:
    explicit FSettings(QObject *parent = nullptr);
    Q_DISABLE_COPY(FSettings)
    ~FSettings();

    void defaultValueInit();

    QSettings *m_settings;
};

#endif // F_SETTINGS_H
