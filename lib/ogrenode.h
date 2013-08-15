/*!
 * \copyright (c) Nokia Corporation and/or its subsidiary(-ies) (qt-info@nokia.com) and/or contributors
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * \license{This source file is part of QmlOgre abd subject to the BSD license that is bundled
 * with this source code in the file LICENSE.}
 */

#ifndef OGRENODE_H
#define OGRENODE_H

#include "Ogre.h"
#include "ogreengine.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGTextureMaterial>
#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QtQuick/QQuickWindow>

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

    void update();
    void updateFBO();

    GLuint getOgreFboId();

    void setOgreEngineItem(OgreEngine *ogreRootItem);
    void doneOgreContext();
    void activateOgreContext();

    void preprocess();

    void setCamera(Ogre::Camera *camera) { m_camera = camera; }

private:
    QSGTextureMaterial m_material;
    QSGOpaqueTextureMaterial m_materialO;
    QSGGeometry m_geometry;
    QSGTexture *m_texture;
    OgreEngine *m_ogreEngineItem;

    QSize m_size;

    Ogre::Camera *m_camera;
    Ogre::RenderTexture *m_renderTexture;
    Ogre::Viewport *m_viewport;
    Ogre::TexturePtr rtt_texture;
    Ogre::RenderWindow *m_window;

    GLuint m_ogreFboId;

    bool m_dirtyFBO;
};

#endif // OGRENODE_H
