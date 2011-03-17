QT += network

include(externalplugin.pri)

INCLUDEPATH += $$QT_SOURCE_TREE/include
INCLUDEPATH += $$QT_SOURCE_TREE/include/QtCore
INCLUDEPATH += ../common/

TARGET = ambiente

SOURCES =   main.cpp \
            qambienteintegration.cpp \
            qambientewindowsurface.cpp \
            windowsystemserver.cpp
HEADERS =   qambienteintegration.h \
            qambientewindowsurface.h \
            windowsystemserver.h \
            ../common/protocol.h

target.path += $$[QT_INSTALL_PLUGINS]/platforms
INSTALLS += target
