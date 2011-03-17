TEMPLATE = app
QT += network

TARGET = ambiente
INCLUDEPATH += ../common/

include(windowsystem/windowsystem.pri)

SOURCES += main.cpp
HEADERS += ../common/protocol.h
