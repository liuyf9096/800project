#include "f_init.h"
#include "f_common.h"
#include "messagecenter.h"
#include "settings/f_settings.h"
#include "logger/f_log_server.h"

#include <QObject>
#include <QString>
#include <QDebug>

void servers_init()
{
    FLogServer::GetInstance();
    FCommon::GetInstance()->printSystemInfo();

    /* log */
    setLog();

    MessageCenter::GetInstance()->connectServer();
    FSettings::GetInstance();
    FLogServer::GetInstance();
}

void setLog()
{
    auto settings = FSettings::GetInstance();
    auto logger = FLogServer::GetInstance();
    logger->setMaxLogFileCount(settings->MaxLogFileCount());
    logger->setMessageFlags(FLogServer::Content);

    if (settings->isLogServerEnable()) {
        logger->saveToTxtEnable(true, settings->logindex());
        logger->start();
    }
}

