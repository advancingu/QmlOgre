/*!
 * \copyright (c) Nokia Corporation and/or its subsidiary(-ies) (qt-info@nokia.com) and/or contributors
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * \license{This source file is part of QmlOgre abd subject to the BSD license that is bundled
 * with this source code in the file LICENSE.}
 */

#include "ogreengine.h"
#include "ogreitem.h"

#include <QOpenGLFunctions>

OgreEngine::OgreEngine(QQuickWindow *window)
    : QObject()
{
    qmlRegisterType<OgreItem>("Ogre", 1, 0, "OgreItem");
    qmlRegisterType<OgreEngine>("OgreEngine", 1, 0, "OgreEngine");

    setQuickWindow(window);
}

OgreEngine::~OgreEngine()
{
    delete m_ogreContext;
}

Ogre::Root* OgreEngine::startEngine()
{
    activateOgreContext();

    Ogre::Root *ogreRoot = new Ogre::Root;
    loadOgrePlugin(ogreRoot, "RenderSystem_GL");

    Ogre::RenderSystem *renderSystem = ogreRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
    ogreRoot->setRenderSystem(renderSystem);
    ogreRoot->initialise(false);

    Ogre::NameValuePairList params;

    params["externalGLControl"] = "true";
    params["currentGLContext"] = "true";

    //Finally create our window.
    m_ogreWindow = ogreRoot->createRenderWindow("OgreWindow", 1, 1, false, &params);
    m_ogreWindow->setVisible(false);
    m_ogreWindow->update(false);

    doneOgreContext();

    return ogreRoot;
}

void OgreEngine::stopEngine(Ogre::Root *ogreRoot)
{
    if (ogreRoot) {
//        m_root->detachRenderTarget(m_renderTexture);
        // TODO tell node(s) to detach

    }

    delete ogreRoot;
}

void OgreEngine::setQuickWindow(QQuickWindow *window)
{
    Q_ASSERT(window);

    m_quickWindow = window;
    m_qtContext = QOpenGLContext::currentContext();

    // create a new shared OpenGL context to be used exclusively by Ogre
    m_ogreContext = new QOpenGLContext();
    m_ogreContext->setFormat(m_quickWindow->requestedFormat());
    m_ogreContext->setShareContext(m_qtContext);
    m_ogreContext->create();
}

void OgreEngine::activateOgreContext()
{
    glPopAttrib();

    m_qtContext->functions()->glUseProgram(0);
    m_qtContext->doneCurrent();

    m_ogreContext->makeCurrent(m_quickWindow);
}

void OgreEngine::doneOgreContext()
{
    m_ogreContext->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_ogreContext->functions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_ogreContext->functions()->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    m_ogreContext->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
    m_ogreContext->doneCurrent();

    m_qtContext->makeCurrent(m_quickWindow);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
}

QOpenGLContext* OgreEngine::ogreContext() const
{
    return m_ogreContext;
}

QSGTexture* OgreEngine::createTextureFromId(uint id, const QSize &size, QQuickWindow::CreateTextureOptions options) const
{
    return m_quickWindow->createTextureFromId(id, size, options);
}

void OgreEngine::loadOgrePlugin(Ogre::Root* ogreRoot, const QString &name)
{
    QString pluginDir = QLatin1String(OGRE_PLUGIN_DIR);
    pluginDir.remove("\"");
    pluginDir += "/";

    QString pluginPath;
#ifdef DEBUG_PLUGIN
    glPlugin += pluginDir + name + QString("_d");
#else
    pluginPath += pluginDir + name;
#endif
    ogreRoot->loadPlugin(pluginPath.toLatin1().constData());
}
