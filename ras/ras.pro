TEMPLATE = app

QT += qml quick

LIBS += -lpgm

SOURCES += main.cpp \
    test.cpp \
    pgm_rx.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    test.h \
    pgm_rx.h
