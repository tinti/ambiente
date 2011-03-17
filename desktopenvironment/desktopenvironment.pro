TEMPLATE = app
QT += network

TARGET = ambiente
INCLUDEPATH += ../common/

HEADERS += ../common/protocol.h server.h window.h
SOURCES += main.cpp server.cpp window.cpp
