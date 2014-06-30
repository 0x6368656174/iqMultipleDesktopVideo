#include <QApplication>
#include <QFile>
#include "mdvmainwindow.h"
#include "mdvplayerapplication.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    qInstallMsgHandler(loggerHandler);

    QApplication a(argc, argv);
    QApplication::setApplicationName("MDPlayer");
    QApplication::setOrganizationName("QuasarSystem");

    //Разберем параметры командной строки
    QList<QString> appArguments = QCoreApplication::arguments();
    if (appArguments.contains("--debug"))
        MDVPlayerApplication::setDebug(true);
    bool autoStartPlaying = appArguments.contains("--autoPlay");
    QString projectName = appArguments.last();
    if (!QFile::exists(projectName))
        projectName = "";

    //Загрузим переводы
    MDVPlayerApplication::reloadTranslations();

    MDVMainWindow w;
    if (!projectName.isEmpty())
        w.loadProject(projectName, autoStartPlaying);

    w.show();    
    return a.exec();
}
