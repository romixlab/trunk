TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    singleinstance.cpp

RESOURCES += qml.qrc \
    images.qrc

RC_ICONS = images/50.png

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    singleinstance.h
