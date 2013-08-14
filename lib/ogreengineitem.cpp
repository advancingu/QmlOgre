#include "ogreengineitem.h"

#include <QOpenGLFunctions>

OgreEngineItem::OgreEngineItem(QQuickWindow *window)
    : QObject()
{
    setQuickWindow(window);
}

OgreEngineItem::~OgreEngineItem()
{
    delete m_ogreContext;
}

Ogre::Root* OgreEngineItem::startEngine()
{
    activateOgreContext();

    Ogre::Root *ogreRoot = new Ogre::Root;
    QString glPlugin = QLatin1String(OGRE_PLUGIN_DIR);
    glPlugin.remove("\"");
#ifdef DEBUG_PLUGIN
    glPlugin += QLatin1String("/RenderSystem_GL_d");
#else
    glPlugin += QLatin1String("/RenderSystem_GL");
#endif
    ogreRoot->loadPlugin(glPlugin.toLatin1().constData());

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

void OgreEngineItem::stopEngine(Ogre::Root *ogreRoot)
{
    if (ogreRoot) {
//        m_root->detachRenderTarget(m_renderTexture);
        // TODO tell node(s) to detach

    }

    delete ogreRoot;
}

void OgreEngineItem::setQuickWindow(QQuickWindow *window)
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

void OgreEngineItem::activateOgreContext()
{
    glPopAttrib();

    m_qtContext->functions()->glUseProgram(0);
    m_qtContext->doneCurrent();

    m_ogreContext->makeCurrent(m_quickWindow);
}

void OgreEngineItem::doneOgreContext()
{
    m_ogreContext->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_ogreContext->functions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    m_ogreContext->functions()->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    m_ogreContext->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
    m_ogreContext->doneCurrent();

    m_qtContext->makeCurrent(m_quickWindow);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
}

QOpenGLContext* OgreEngineItem::ogreContext() const
{
    return m_ogreContext;
}

QSGTexture* OgreEngineItem::createTextureFromId(uint id, const QSize &size, QQuickWindow::CreateTextureOptions options) const
{
    return m_quickWindow->createTextureFromId(id, size, options);
}
