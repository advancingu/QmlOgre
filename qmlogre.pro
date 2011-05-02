CONFIG += qt
QT += opengl declarative
TEMPLATE = app
TARGET = qmlogre

macx {
    OGREDIR = $$(OGRE_HOME)
    isEmpty(OGREDIR) {
        error(QmlOgre needs Ogre to be built. Please set the environment variable OGRE_HOME pointing to your Ogre root directory.)
    } else {
        message(Using Ogre libraries in $$OGREDIR)
        INCLUDEPATH += $$OGREDIR/include/OGRE
        QMAKE_LFLAGS += -F$$OGREDIR/lib/release
        LIBS += -framework Ogre

        BOOSTDIR = $$OGREDIR/boost_1_42
        !isEmpty(BOOSTDIR) {
            INCLUDEPATH += $$BOOSTDIR
            LIBS += -L$$BOOSTDIR/lib -lboost_date_time-xgcc40-mt-1_42 -lboost_thread-xgcc40-mt-1_42
        }

        DEFINES += OGRE_PLUGIN_VAR=\"$$OGREDIR/lib\"
    }
} else:unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += OGRE
    OGRELIBDIR = $$system(pkg-config --libs-only-L OGRE)
    isEmpty(OGRELIBDIR) {
        OGRELIBDIR = /usr/lib
    } else {
        OGRELIBDIR = $$replace(OGRELIBDIR, -L,)
    }
    OGREPLUGINDIR = $$OGRELIBDIR/OGRE
    DEFINES += OGRE_PLUGIN_VAR=\"$$OGREPLUGINDIR\"
} else:win32 {
    OGREDIR = $$(OGRE_HOME)
    isEmpty(OGREDIR) {
        error(QmlOgre needs Ogre to be built. Please set the environment variable OGRE_HOME pointing to your Ogre root directory.)
    } else {
        message(Using Ogre libraries in $$OGREDIR)
        INCLUDEPATH += $$OGREDIR/include/OGRE
        CONFIG(release, debug|release) {
            LIBS += -L$$OGREDIR/lib/release -L$$OGREDIR/lib/release/opt -lOgreMain -lRenderSystem_GL
        } else {
            LIBS += -L$$OGREDIR/lib/debug -L$$OGREDIR/lib/debug/opt -lOgreMain_d -lRenderSystem_GL_d
        }

        BOOSTDIR = $$OGREDIR/boost_1_42
        !isEmpty(BOOSTDIR) {
            INCLUDEPATH += $$BOOSTDIR
            CONFIG(release, debug|release) {
                LIBS += -L$$BOOSTDIR/lib -llibboost_date_time-vc90-mt-1_42 -llibboost_thread-vc90-mt-1_42
            } else {
                LIBS += -L$$BOOSTDIR/lib -llibboost_date_time-vc90-mt-gd-1_42 -llibboost_thread-vc90-mt-gd-1_42
            }
        }

        CONFIG(release, debug|release) {
            DEFINES += OGRE_PLUGIN_VAR=\\\"$$OGREDIR/bin/release\\\"
        } else {
            DEFINES += OGRE_PLUGIN_VAR=\\\"$$OGREDIR/bin/debug\\\"
            DEFINES += DEBUG_PLUGIN=1
        }
    }
}

UI_DIR = ./.ui
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc


SOURCES += main.cpp \
    cameranodeobject.cpp \
    ogreitem.cpp \
    ogrenode.cpp

HEADERS += \
    cameranodeobject.h \
    ogreitem.h \
    ogrenode.h

OTHER_FILES += \
    resources/example.qml
