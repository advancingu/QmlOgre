#ifndef EXAMPLEAPP_H
#define EXAMPLEAPP_H

#include "../lib/ogreengineitem.h"

#include <QtQuick/QQuickView>

class ExampleApp : public QQuickView
{
    Q_OBJECT
public:
    explicit ExampleApp(QWindow *parent = 0);
    ~ExampleApp();
    
signals:
    
public slots:
    
private:
    OgreEngineItem *m_ogreEngineItem;
    Ogre::SceneManager *m_sceneManager;
};

#endif // EXAMPLEAPP_H
