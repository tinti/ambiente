QT += network

include(externalplugin.pri)

INCLUDEPATH += $$QT_SOURCE_TREE/include
INCLUDEPATH += $$QT_SOURCE_TREE/include/QtCore
INCLUDEPATH += ../common/

TARGET = ambiente

SOURCES =   main.cpp \
            ambienteintegration.cpp \
            ambientewindowsurface.cpp \
            ambientescreen.cpp \
            windowsystemserver.cpp

HEADERS =   ambienteintegration.h \
            ambientewindowsurface.h \
            ambientescreen.h \
            windowsystemserver.h \
            ../common/protocol.h

target.path += $$[QT_INSTALL_PLUGINS]/platforms
INSTALLS += target
