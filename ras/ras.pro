TEMPLATE = app

QT += qml quick

LIBS += -lpgm

SOURCES += main.cpp \
    pgmnetwork.cpp \
    test.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    pgmnetwork.h \
    test.h
