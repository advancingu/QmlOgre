#include "ogrewidget.h"
#include <QtGui/QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    OgreWidget w;
    w.show();

    return app.exec();
}
