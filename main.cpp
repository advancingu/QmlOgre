#include "ogreitem.h"
#include <QtGui/QApplication>
#include <QSGView>
#include <QDeclarativeContext>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    qmlRegisterType<OgreItem>("Ogre", 1, 0, "OgreItem");

    QGLFormat f = QGLFormat::defaultFormat();
    f.setSampleBuffers(true);
    QSGView view(f);
    view.setResizeMode(QSGView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("resources/example.qml"));
    view.rootContext()->setContextProperty("Window", &view);
    view.show();
    view.raise();

    return app.exec();
}
