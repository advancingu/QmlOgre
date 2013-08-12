CONFIG += qt
QT += qml quick
TEMPLATE = app
TARGET = qmlogre

SOURCES += main.cpp \
    cameranodeobject.cpp \
    exampleapp.cpp

HEADERS += cameranodeobject.h \
    exampleapp.h

OTHER_FILES += \
    resources/example.qml
