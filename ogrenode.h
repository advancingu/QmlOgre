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

    void setAAEnabled(bool enable);

    CameraNodeObject *camera() const { return m_cameraObject; }

    void update();
    void updateFBO();

    void init();

    GLuint getOgreFBO();

    void saveOgreState();
    void restoreOgreState();

    void preprocess();

private:
    QSGTextureMaterial m_material;
    QSGTextureMaterialWithOpacity m_materialO;
    QSGGeometry m_geometry;
    QSGPlainTexture *m_texture;

    int m_samples;
    bool m_AAEnabled;
    QSize m_size;

    Ogre::Root *m_root;
    Ogre::Camera *m_camera;
    Ogre::SceneManager *m_sceneManager;
    Ogre::RenderTexture *m_renderTexture;
    Ogre::Viewport *m_viewport;
    Ogre::TexturePtr rtt_texture;
    Ogre::RenderWindow *m_window;

    GLuint m_ogreFBO;

    CameraNodeObject *m_cameraObject;

    bool m_initialized;
    bool m_dirtyFBO;
};

#endif // OGRENODE_H
