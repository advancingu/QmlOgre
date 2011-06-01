/***************************************************************************
**
** This file is part of the qmlogre example on http://qt.gitorious.org.
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).*
**
** All rights reserved.
** Contact:  Nokia Corporation (qt-info@nokia.com)
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**  * Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
**  * Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with ** the distribution.
**  * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor the
**    names of its contributors may be used to endorse or promote products
**    derived from this software without specific ** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**************************************************************************/

#include <RenderSystems/GL/OgreGLTexture.h>
#include <RenderSystems/GL/OgreGLFrameBufferObject.h>
#include <RenderSystems/GL/OgreGLFBORenderTexture.h>

#include "ogrenode.h"
#include "cameranodeobject.h"

#include <Ogre.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtOpenGL/QGLFunctions>

static QString appPath()
{
    QString path = QCoreApplication::applicationDirPath();
    QDir dir(path);
#ifdef Q_WS_MAC
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
#elif defined(Q_WS_WIN)
    dir.cdUp();
#endif
    return dir.absolutePath();
}

OgreNode::OgreNode()
    : QSGGeometryNode()
    , m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    , m_texture(0)
    , m_samples(0)
    , m_AAEnabled(false)
    , m_renderTexture(0)
    , m_ogreFBO(0)
    , m_initialized(false)
    , m_dirtyFBO(false)
{
    setMaterial(&m_materialO);
    setOpaqueMaterial(&m_material);
    setGeometry(&m_geometry);
    setFlag(UsePreprocess);
}

OgreNode::~OgreNode()
{
    if (m_renderTexture) {
        m_renderTexture->removeAllViewports();
    }

    if (m_root) {
        m_root->detachRenderTarget(m_renderTexture);

        if (m_sceneManager) {
            m_root->destroySceneManager(m_sceneManager);
        }
    }

    delete m_root;
}

void OgreNode::saveOgreState()
{
    m_ogreFBO = getOgreFBO();

    const QGLContext *ctx = QGLContext::currentContext();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    ctx->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    ctx->functions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ctx->functions()->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}

void OgreNode::restoreOgreState()
{
    const QGLContext *ctx = QGLContext::currentContext();
    glPopAttrib();
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_ogreFBO);
    ctx->functions()->glUseProgram(0);
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

    const QGLContext *ctx = QGLContext::currentContext();
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);

    return id;
}

void OgreNode::preprocess()
{
    restoreOgreState();
    m_renderTexture->update(true);
    saveOgreState();
}

void OgreNode::update()
{
    restoreOgreState();

    if (!m_initialized)
        init();

    if (m_dirtyFBO)
        updateFBO();

    saveOgreState();
}

void OgreNode::updateFBO()
{
    if (m_renderTexture)
        Ogre::TextureManager::getSingleton().remove("RttTex");

    rtt_texture = Ogre::TextureManager::getSingleton().createManual("RttTex",
                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                    Ogre::TEX_TYPE_2D,
                                                                    m_size.width(),
                                                                    m_size.height(),
                                                                    0,
                                                                    Ogre::PF_R8G8B8A8,
                                                                    Ogre::TU_RENDERTARGET, 0, false,
                                                                    m_AAEnabled ? m_samples : 0);

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
    m_texture = new QSGPlainTexture;
    m_texture->setOwnsTexture(false);
    m_texture->setTextureSize(m_size);
    m_texture->setTextureId(nativeTexture->getGLID());

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

void OgreNode::setAAEnabled(bool enable)
{
    if (m_AAEnabled == enable)
        return;

    m_AAEnabled = enable;
    m_dirtyFBO = true;
    markDirty(DirtyMaterial);
}

void OgreNode::init()
{
    const QGLContext *ctx = QGLContext::currentContext();
    QGLFormat format = ctx->format();
    m_samples = format.sampleBuffers() ? format.samples() : 0;

    m_root = new Ogre::Root;
    QString glPlugin = QLatin1String(OGRE_PLUGIN_DIR);
    glPlugin.remove("\"");
#ifdef DEBUG_PLUGIN
    glPlugin += QLatin1String("/RenderSystem_GL_d");
#else
    glPlugin += QLatin1String("/RenderSystem_GL");
#endif
    m_root->loadPlugin(glPlugin.toLatin1().constData());

    Ogre::RenderSystem *renderSystem = m_root->getRenderSystemByName("OpenGL Rendering Subsystem");
    m_root->setRenderSystem(renderSystem);
    m_root->initialise(false);

    Ogre::NameValuePairList params;

    params["externalGLControl"] = "true";
    params["currentGLContext"] = "true";

    //Finally create our window.
    m_window = m_root->createRenderWindow("OgreWindow", 1, 1, false, &params);
    m_window->setVisible(false);
    m_window->update(false);

    // Load resources
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(QString(appPath() + "/resources/data.zip").toAscii().data(), "Zip");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Setup scene
    m_sceneManager = m_root->createSceneManager(Ogre::ST_GENERIC, "mySceneManager");
    m_camera = m_sceneManager->createCamera("myCamera");
    m_camera->setNearClipDistance(1);
    m_camera->setFarClipDistance(99999);
    m_camera->setAspectRatio(Ogre::Real(m_size.width()) / Ogre::Real(m_size.height()));

    // Setup content...

    // Set a sky dome
    m_sceneManager->setSkyBox(true, "SpaceSkyBox", 10000);

    // setup some basic lighting for our scene
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
    m_sceneManager->createLight("myLight")->setPosition(20, 80, 50);

    // create an ogre head entity and place it at the origin
    m_sceneManager->getRootSceneNode()->attachObject(m_sceneManager->createEntity("Head", "ogrehead.mesh"));

    // Setup the camera
    m_cameraObject = new CameraNodeObject(m_camera);
    m_cameraObject->camera()->setAutoTracking(true, m_sceneManager->getRootSceneNode());

    m_initialized = true;
}
