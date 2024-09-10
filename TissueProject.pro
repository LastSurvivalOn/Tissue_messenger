QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    barwidget.cpp \
    chatswindow.cpp \
    chatusers.cpp \
    listelement.cpp \
    main.cpp \
    mainwindow.cpp \
    messagechat.cpp \
    tissue_server.cpp \
    useraccount.cpp

HEADERS += \
    barwidget.h \
    chatswindow.h \
    chatusers.h \
    listelement.h \
    mainwindow.h \
    messagechat.h \
    tissue_server.h \
    useraccount.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
