CONFIG += qt
QT += opengl declarative
TEMPLATE = app
TARGET = qmlogre

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += OGRE
    OGRELIBDIR = $$system(pkg-config --libs-only-L OGRE)
    OGRELIBDIR = $$replace(OGRELIBDIR, -L,)
    OGREPLUGINDIR = $$OGRELIBDIR/OGRE
    DEFINES += OGRE_PLUGIN_VAR=\"$$OGREPLUGINDIR\"
} else {
    OGREDIR = $$(OGRE_HOME)
    isEmpty(OGREDIR) {
        error(QmlOgre needs Ogre to be built. Please set the environment variable OGRE_HOME pointing to your Ogre root directory.)
    } else {
        message(Using Ogre libraries in $$OGREDIR)
        INCLUDEPATH += $$OGREDIR/include/OGRE
        macx {
            QMAKE_LFLAGS += -F$$OGREDIR/lib/release
            LIBS += -framework Ogre
        } else {
            LIBS += -L$$OGREDIR/lib -lOgreMain
        }
    }

    BOOSTDIR = $$OGREDIR/boost_1_42
    !isEmpty(BOOSTDIR) {
        INCLUDEPATH += $$BOOSTDIR
        LIBS += -L$$BOOSTDIR/lib -lboost_date_time-xgcc40-mt-1_42 -lboost_thread-xgcc40-mt-1_42
    }

    DEFINES += OGRE_PLUGIN_VAR=$$OGREDIR/lib
}

UI_DIR = ./.ui
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc


SOURCES += main.cpp \
    ogrewidget.cpp \
    declarativeviewtexture.cpp

HEADERS += \
    ogrewidget.h \
    declarativeviewtexture.h
