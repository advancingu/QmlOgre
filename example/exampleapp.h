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
    
signals:
    
public slots:
    void addContent();
    
private:
    CameraNodeObject *m_cameraObject;
    OgreEngineItem *m_ogreEngineItem;

    Ogre::SceneManager *m_sceneManager;
    Ogre::Camera *m_camera;
    Ogre::Root *m_root;
};

#endif // EXAMPLEAPP_H
