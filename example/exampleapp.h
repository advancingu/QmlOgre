#ifndef EXAMPLEAPP_H
#define EXAMPLEAPP_H

#include "../lib/ogreengineitem.h"
#include "cameranodeobject.h"

#include <QtQuick/QQuickView>

class ExampleApp : public QQuickView
{
    Q_OBJECT
public:
    explicit ExampleApp(QWindow *parent = 0);
    ~ExampleApp();
    void addContent();
    
signals:
    
public slots:
    
private:
    CameraNodeObject *m_cameraObject;
    OgreEngineItem *m_ogreEngineItem;

    Ogre::SceneManager *m_sceneManager;
    Ogre::Camera *m_camera;
};

#endif // EXAMPLEAPP_H
