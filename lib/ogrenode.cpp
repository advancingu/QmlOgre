/*!
 * \copyright (c) Nokia Corporation and/or its subsidiary(-ies) (qt-info@nokia.com) and/or contributors
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * \license{This source file is part of QmlOgre abd subject to the BSD license that is bundled
 * with this source code in the file LICENSE.}
 */

#include <RenderSystems/GL/OgreGLTexture.h>
#include <RenderSystems/GL/OgreGLFrameBufferObject.h>
#include <RenderSystems/GL/OgreGLFBORenderTexture.h>

#include "ogrenode.h"

#include <Ogre.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

OgreNode::OgreNode()
    : QSGGeometryNode()
    , m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    , m_texture(0)
    , m_ogreEngineItem(0)
    , m_camera(0)
    , m_renderTexture(0)
    , m_viewport(0)
    , m_window(0)
    , m_ogreFBO(0)
    , m_dirtyFBO(false)
{
    setMaterial(&m_material);
    setOpaqueMaterial(&m_materialO);
    setGeometry(&m_geometry);
    setFlag(UsePreprocess);
}

OgreNode::~OgreNode()
{
    if (m_renderTexture) {
        m_renderTexture->removeAllViewports();
    }
}

void OgreNode::setOgreEngineItem(OgreEngineItem *ogreRootItem)
{
    m_ogreEngineItem = ogreRootItem;
}

void OgreNode::doneOgreContext()
{
    m_ogreEngineItem->doneOgreContext();
}

void OgreNode::activateOgreContext()
{
    m_ogreEngineItem->activateOgreContext();
    m_ogreEngineItem->ogreContext()->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_ogreFBO);
}

GLuint OgreNode::getOgreFBO()
{
    if (!m_renderTexture)
        return 0;

    Ogre::GLFrameBufferObject *ogreFbo = 0;
    m_renderTexture->getCustomAttribute("FBO", &ogreFbo);
    Ogre::GLFBOManager *manager = ogreFbo->getManager();
    manager->bind(m_renderTexture);

    GLint id;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &id);

    return id;
}

void OgreNode::preprocess()
{
    activateOgreContext();
    m_renderTexture->update(true);
    doneOgreContext();
}

void OgreNode::update()
{
    if (m_dirtyFBO) {
        activateOgreContext();
        updateFBO();
        m_ogreFBO = getOgreFBO();
        m_dirtyFBO = false;
        doneOgreContext();
    }
}

void OgreNode::updateFBO()
{
    if (m_renderTexture)
        Ogre::TextureManager::getSingleton().remove("RttTex");

    int samples = m_ogreEngineItem->ogreContext()->format().samples();
    rtt_texture = Ogre::TextureManager::getSingleton().createManual("RttTex",
                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                    Ogre::TEX_TYPE_2D,
                                                                    m_size.width(),
                                                                    m_size.height(),
                                                                    0,
                                                                    Ogre::PF_R8G8B8A8,
                                                                    Ogre::TU_RENDERTARGET, 0, false,
                                                                    samples);

    m_renderTexture = rtt_texture->getBuffer()->getRenderTarget();

    m_renderTexture->addViewport(m_camera);
    m_renderTexture->getViewport(0)->setClearEveryFrame(true);
    m_renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
    m_renderTexture->getViewport(0)->setOverlaysEnabled(false);

    Ogre::Real aspectRatio = Ogre::Real(m_size.width()) / Ogre::Real(m_size.height());
    m_camera->setAspectRatio(aspectRatio);

    QSGGeometry::updateTexturedRectGeometry(&m_geometry,
                                            QRectF(0, 0, m_size.width(), m_size.height()),
                                            QRectF(0, 0, 1, 1));

    Ogre::GLTexture *nativeTexture = static_cast<Ogre::GLTexture *>(rtt_texture.get());

    delete m_texture;
    m_texture = m_ogreEngineItem->createTextureFromId(nativeTexture->getGLID(), m_size);

    m_material.setTexture(m_texture);
    m_materialO.setTexture(m_texture);
}

void OgreNode::setSize(const QSize &size)
{
    if (size == m_size)
        return;

    m_size = size;
    m_dirtyFBO = true;
    markDirty(DirtyGeometry);
}
