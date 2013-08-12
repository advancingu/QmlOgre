#include "ogrerootitem.h"

OgreRootItem::OgreRootItem()
{
    const QOpenGLContext *ctx = QOpenGLContext::currentContext();
    QSurfaceFormat format = ctx->format();
    m_samples = format.samples();

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
}

OgreRootItem::~OgreRootItem()
{
    if (m_root) {
        m_root->detachRenderTarget(m_renderTexture);

        if (m_sceneManager) {
            m_root->destroySceneManager(m_sceneManager);
        }
    }

    delete m_root;

    delete m_ogreContext;
}

void OgreRootItem::setQuickWindow(QQuickWindow *window)
{
    m_quickWindow = window;

    // create a new shared OpenGL context to be used exclusively by Ogre
    m_ogreContext = new QOpenGLContext();
    m_ogreContext->setFormat(m_quickWindow->requestedFormat());
    m_ogreContext->setShareContext(QOpenGLContext::currentContext());
    m_ogreContext->create();
}
