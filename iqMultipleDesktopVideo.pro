#-------------------------------------------------
#
# Project created by QtCreator 2012-12-13T04:20:10
#
#-------------------------------------------------

QT       += core gui phonon xml declarative sql

CONFIG   += exception

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = multipleDesktopVideo
TEMPLATE = app


SOURCES += main.cpp\
        mdvmainwindow.cpp \
    screentab.cpp \
    mytabwidget.cpp \
    mediatablemodel.cpp \
    media.cpp \
    mediasettingsgroupbox.cpp \
    saveloadinterface.cpp \
    screen.cpp \
    desktopmediaplayer.cpp \
    aboutdialog.cpp \
    extensions/kinoafisha/kinoafishawidget.cpp \
    extensions/kinoafisha/cinema.cpp \
    extensions/extensionsqlconfigwidget.cpp \
    extensions/abstarctextension.cpp \
    extensions/kinoafisha/configwidget.cpp \
    extensionsengine.cpp \
    extensions/kinoafisha/kinoafishaextension.cpp \
    extensionsconfigdialog.cpp \
    extensions/abstractextentionconfigwidget.cpp \
    mdvplayerapplication.cpp \
    logdialog.cpp \
    extensions/kinoafisha/qmlvideo.cpp \
    mediaplayer.cpp \
    mediapreview.cpp \
    configdialog.cpp

HEADERS  += mdvmainwindow.h \
    screentab.h \
    mytabwidget.h \
    mediatablemodel.h \
    media.h \
    mediasettingsgroupbox.h \
    saveloadinterface.h \
    screen.h \
    desktopmediaplayer.h \
    aboutdialog.h \
    extensions/kinoafisha/kinoafishawidget.h \
    extensions/kinoafisha/cinema.h \
    extensions/extensionsqlconfigwidget.h \
    extensions/abstarctextension.h \
    extensions/kinoafisha/configwidget.h \
    extensionsengine.h \
    extensions/kinoafisha/kinoafishaextension.h \
    extensionsconfigdialog.h \
    extensions/abstractextentionconfigwidget.h \
    extensions/extensionsqlconfigwidgetdefines.h \
    extensions/kinoafisha/kinoafishadefines.h \
    mdvplayerapplication.h \
    logger.h \
    logdialog.h \
    extensions/kinoafisha/qmlvideo.h \
    mediaplayer.h \
    mediapreview.h \
    configdialog.h \
    settingsdefines.h

FORMS    += mdvmainwindow.ui \
    screentab.ui \
    mediasettingsgroupbox.ui \
    aboutdialog.ui \
    extensions/extensionsqlconfigwidget.ui \
    extensions/kinoafisha/configwidget.ui \
    extensionsconfigdialog.ui \
    logdialog.ui \
    configdialog.ui

RESOURCES += \
    icons.qrc

TRANSLATIONS += \
    mdplayer_ru.ts \
    mdplayer_qml_ru.ts
    
RC_FILE = mdplayer.rc

OTHER_FILES += \
    extensions/kinoafisha/qml/Movie.qml \
    extensions/kinoafisha/qml/CinemaHall.qml \
    extensions/kinoafisha/qml/Affisha.qml \
    extensions/kinoafisha/qml/CinemaSeance.qml \
    extensions/kinoafisha/qml/Cinema.qml \
    extensions/kinoafisha/qml/Page.qml \
    extensions/kinoafisha/qml/TrailerVideoPlayer.qml \
    extensions/kinoafisha/qml/Trailers.qml \
    qml/VideoPlayer.qml \
    extensions/kinoafisha/qml/Style.qml
