TEMPLATE = app

QT += qml quick

QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += /home/roman/Qt/5.3/gcc_64/include/QtCore/5.3.1/QtCore/private/

LIBS += -lqmsgpack
LIBS += -L"/home/roman/build/yami4-gpl-1.10.0/lib/" -lyamicpp -lyamicore

SOURCES += main.cpp \
    localdiscoveryservice.cpp \
    test.cpp \
    yami.cpp \
    automation.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += localdiscoveryservice.h \
    discoveryservicebase.h \
    automation_global.h \
    automation_export.h \
    test.h \
    ToCatch.h \
    yami.h \
    automation.h
