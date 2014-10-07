TEMPLATE = app

QT += qml quick

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lqmsgpack

SOURCES += main.cpp \
    localdiscoveryservice.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += localdiscoveryservice.h \
    discoveryservicebase.h \
    automation_global.h \
    automation_export.h
