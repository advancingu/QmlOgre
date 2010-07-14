#include "ogrewidget.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreColourValue.h>
#include <OgreWindowEventUtilities.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreResourceGroupManager.h>
#include <QtGui/QResizeEvent>
#include <QtCore/QDebug>

#if defined(Q_WS_X11)
#include <QX11Info>
#endif

OgreWidget::OgreWidget(QWidget *parent) :
    QWidget(parent),
    m_root(0),
    m_camera(0),
    m_sceneManager(0),
    m_renderWindow(0),
    m_viewport(0)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    resize(1024, 768);

    initOgre();

    startTimer(16);
}

OgreWidget::~OgreWidget()
{
    if (m_renderWindow) {
        m_renderWindow->removeAllViewports();
    }

    if (m_root) {
        m_root->detachRenderTarget(m_renderWindow);

        if (m_sceneManager) {
            m_root->destroySceneManager(m_sceneManager);
        }
    }

    delete m_root;
}

void OgreWidget::initOgre()
{
    m_root = new Ogre::Root;
    m_root->loadPlugin(Ogre::String(OGRE_PLUGIN_DIR) + "/RenderSystem_GL");

    Ogre::RenderSystem *renderSystem = m_root->getRenderSystemByName("OpenGL Rendering Subsystem");
    m_root->setRenderSystem(renderSystem);
    m_root->initialise(false);

    Ogre::NameValuePairList params;
    Ogre::String externalWindowHandleParams;

    //Accept input focus
    //setFocusPolicy(Qt::StrongFocus);

#if defined(Q_WS_WIN)
    //positive integer for W32 (HWND handle) - According to Ogre Docs
    externalWindowHandleParams = Ogre::StringConverter::toString((unsigned int)(winId()));
#endif

#if defined(Q_WS_X11)
    QX11Info info = x11Info();
    externalWindowHandleParams  = Ogre::StringConverter::toString((unsigned long)(info.display()));
    externalWindowHandleParams += ":";
    externalWindowHandleParams += Ogre::StringConverter::toString((unsigned int)(info.screen()));
    externalWindowHandleParams += ":";
    externalWindowHandleParams += Ogre::StringConverter::toString((unsigned long)(winId()));
#endif

    //Add the external window handle parameters to the existing params set.
#if defined(Q_WS_WIN)
    params["externalWindowHandle"] = externalWindowHandleParams;
#endif

#if defined(Q_WS_X11)
    params["parentWindowHandle"] = externalWindowHandleParams;
#endif

    //Finally create our window.
    m_renderWindow = m_root->createRenderWindow("OgreWindow", width(), height(), false, &params);
    m_renderWindow->setVisible(true);

    // Load resources
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("data.zip", "Zip");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Setup scene
    m_sceneManager = m_root->createSceneManager(Ogre::ST_GENERIC, "mySceneManager");
    m_camera = m_sceneManager->createCamera("myCamera");
    m_camera->setNearClipDistance(200);
    m_camera->setFarClipDistance(99999);
    m_viewport = m_renderWindow->addViewport(m_camera);
    m_viewport->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
    m_viewport->setClearEveryFrame(true);
    m_camera->setAspectRatio(Ogre::Real(width()) / Ogre::Real(height()));
    //m_camera->setFOVy(Ogre::Radian(Ogre::Degree(20)));

    // Setup content...

    // Set a sky dome
    m_sceneManager->setSkyBox(true, "SpaceSkyBox", 10000);

    // setup some basic lighting for our scene
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
    m_sceneManager->createLight()->setPosition(20, 80, 50);

    // create an ogre head entity and place it at the origin
    m_sceneManager->getRootSceneNode()->attachObject(m_sceneManager->createEntity("Head", "ogrehead.mesh"));
    m_camera->move(Ogre::Vector3(0, 0, 300));
    m_camera->lookAt(0, 0, 0);
}

void OgreWidget::render()
{
    Ogre::WindowEventUtilities::messagePump();
    m_root->renderOneFrame();
}

void OgreWidget::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e)
    render();
}

void OgreWidget::resizeEvent(QResizeEvent *e)
{
    const QSize &newSize = e->size();
    if (m_renderWindow) {
        m_renderWindow->resize(newSize.width(), newSize.height());
        m_renderWindow->windowMovedOrResized();
    }
    if (m_camera) {
        Ogre::Real aspectRatio = Ogre::Real(newSize.width()) / Ogre::Real(newSize.height());
        m_camera->setAspectRatio(aspectRatio);
    }

    QWidget::resizeEvent(e);
}
