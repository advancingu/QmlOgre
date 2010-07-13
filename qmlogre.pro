CONFIG += qt
QT += opengl
TEMPLATE = app
TARGET = qmlogre

OGREDIR = $$(OGRE_HOME)
isEmpty(OGREDIR) {
    error(QmlOgre needs Ogre to be built. Please set the environment variable OGRE_HOME pointing to your Ogre root directory.)
} else {
    message(Using Ogre libraries in $$OGREDIR)
    INCLUDEPATH += $$OGREDIR/include
    macx {
        QMAKE_LFLAGS += -F$$OGREDIR/lib/release
        LIBS += -framework Ogre
    } else {
        LIBS += -L$$OGREDIR/lib -lOgre
    }
}

UI_DIR = ./.ui
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc


SOURCES += main.cpp
