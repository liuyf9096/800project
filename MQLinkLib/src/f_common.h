#ifndef F_COMMON_H
#define F_COMMON_H

#include <QObject>
#include <QJsonObject>

class FCommon : public QObject
{
    Q_OBJECT
public:
    static FCommon *GetInstance();

    static const int Version_major{ 1 };
    static const int Version_minor{ 0 };
    static const int Version_patch{ 0 };

    static void printSystemInfo();

    static QString appName();
    static QString appVersion();
    static QString appFullVersion();
    static QString releaseDate();

    /* path */
    static QString appPath();
    static QString getPath(const QString &dirName);
    static QString checkIpAddress(const QString &address);

    void setVerbose() { m_verbose = true; }
    bool isVerbose() { return m_verbose; }

    QJsonValue getConfigValue(const QString &key);
    QJsonValue getConfigValue(const QString &key1, const QString &key2);
    QJsonValue getConfigValue(const QString &key1, const QString &key2, const QString &key3);
    QJsonValue getConfigValue(const QString &key1, const QString &key2, const QString &key3, const QString &key4);

    static QJsonValue getConfigFileValue(const QString &key);
    static QJsonValue getConfigFileValue(const QString &key1, const QString &key2);
    static QJsonValue getConfigFileValue(const QString &key1, const QString &key2, const QString &key3);
    static QJsonValue getConfigFileValue(const QString &key1, const QString &key2, const QString &key3, const QString &key4);

    static bool setConfigFileValue(const QString &key1, const QString &key2, const QJsonValue &value2);
    static bool setConfigFileValue(const QString &key1, const QString &key2, const QString &key3, const QJsonValue &value3);
    static bool setConfigFileValue(const QString &key1, const QString &key2, const QString &key3, const QString &key4, const QJsonValue &value4);

private:
    explicit FCommon(QObject *parent = nullptr);
    Q_DISABLE_COPY(FCommon)
    ~FCommon();

    QJsonObject m_configObj;
    bool m_verbose;
};

#endif // F_COMMON_H

