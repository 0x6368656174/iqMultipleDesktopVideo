#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QCoreApplication>
#include <QTextCodec>
#include "mdvplayerapplication.h"

void loggerHandler(QtMsgType type, const char *msg)
{
    QString logStr (msg);

#ifdef Q_OS_WIN
    logStr = QTextCodec::codecForName("Windows-1251")->toUnicode(msg);
#endif

    MDVPlayerApplication::printToLog(type, logStr);
}


#endif // LOGGER_H
