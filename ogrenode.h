#ifndef OGRENODE_H
#define OGRENODE_H

#include <QSGGeometryNode>
#include "qsgtexturematerial.h"
#include <private/qsgtexture_p.h>

#include <OgreTexture.h>

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define OGRE_PLUGIN_DIR STRINGIFY(OGRE_PLUGIN_VAR)

namespace Ogre {
class Root;
class Camera;
class SceneManager;
class RenderTexture;
class Viewport;
class RenderTarget;
}

class CameraNodeObject;

class OgreNode : public QSGGeometryNode
{
public:
    OgreNode();
    ~OgreNode();

    void setSize(const QSize &size);
    QSize size() const { return m_size; }

    CameraNodeObject *camera() const { return m_cameraObject; }

    void update();
    void updateSize();

    void init();

    void preprocess();

private:
    QSGTextureMaterial m_material;
    QSGTextureMaterialWithOpacity m_materialO;
    QSGGeometry m_geometry;
    QSGPlainTexture *m_texture;

    QSize m_size;

    Ogre::Root *m_root;
    Ogre::Camera *m_camera;
    Ogre::SceneManager *m_sceneManager;
    Ogre::RenderTexture *m_renderTexture;
    Ogre::Viewport *m_viewport;
    Ogre::TexturePtr rtt_texture;
    Ogre::RenderWindow *m_window;

    CameraNodeObject *m_cameraObject;

    bool m_initialized;
    bool m_dirtySize;
};

#endif // OGRENODE_H
