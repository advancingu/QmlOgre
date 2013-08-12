#ifndef OGREROOTITEM_H
#define OGREROOTITEM_H

namespace Ogre {
class Root;
class Camera;
class SceneManager;
class RenderTexture;
class Viewport;
class RenderTarget;
}

class OgreRootItem
{
public:
    OgreRootItem();
    ~OgreRootItem();

    void setQuickWindow(QQuickWindow *window);

private:
    Ogre::Root *m_root;
    Ogre::Camera *m_camera;
    Ogre::SceneManager *m_sceneManager;
};

#endif // OGREROOTITEM_H
