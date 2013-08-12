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
    , m_renderTexture(0)
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

void OgreNode::doneOgreState()
{
    m_ogreEngineItem->doneOgreState();
}

void OgreNode::activateOgreState()
{
    m_ogreEngineItem->activateOgreState();
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
    activateOgreState();
    m_renderTexture->update(true);
    doneOgreState();
}

void OgreNode::update()
{
    if (m_dirtyFBO) {
        activateOgreState();
        updateFBO();
        m_ogreFBO = getOgreFBO();
        m_dirtyFBO = false;
        doneOgreState();
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
