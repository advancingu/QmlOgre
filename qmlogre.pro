CONFIG += qt
QT += opengl
TEMPLATE = app
TARGET = qmlogre

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += OGRE
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
}

UI_DIR = ./.ui
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc


SOURCES += main.cpp
