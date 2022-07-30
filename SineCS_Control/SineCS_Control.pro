QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calibrationdialog.cpp \
    cs_control.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    calibrationdialog.h \
    cs_control.h \
    mainwindow.h

FORMS += \
    calibrationdialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    SineCS_Control_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += $$PWD/Libs/Includes/
DEPENDPATH += $$PWD/Libs/Includes/

LIBS += -lsetupapi
LIBS += -luser32
LIBS += -L$$PWD/Libs/Src/ -llibusb-1.0

CONFIG(release, debug|release) {
    DESTDIR = release_output
    win32 {
        QMAKE_POST_LINK += windeployqt $$OUT_PWD/$$DESTDIR
    }
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
