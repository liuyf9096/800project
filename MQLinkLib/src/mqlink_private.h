#ifndef MQLINK_PRIVATE_H
#define MQLINK_PRIVATE_H

#include <QtCore/qglobal.h>

#if defined(MQLINKLIB_LIBRARY)
#define MQLINKLIB_EXPORT Q_DECL_EXPORT
#else
#define MQLINKLIB_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

class MQLINKLIB_EXPORT MQLinkPrivate : public QObject
{
    Q_OBJECT
public:
    explicit MQLinkPrivate(QObject *parent = nullptr);
    ~MQLinkPrivate();

private:
    void logInit();
    void downloadPathInit();
};

#endif // MQLINK_PRIVATE_H
