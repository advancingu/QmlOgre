#include "ogrewidget.h"

#include "declarativeviewtexture.h"
#include "cameranodeobject.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreColourValue.h>
#include <OgreWindowEventUtilities.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreResourceGroupManager.h>
#include <OgreMaterialManager.h>
#include <QtDeclarative/QDeclarativeContext>
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

#if defined(Q_WS_X11)
#include <QX11Info>
#endif

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

OgreWidget::OgreWidget(QWidget *parent) :
    QGLWidget(parent),
    m_root(0),
    m_camera(0),
    m_sceneManager(0),
    m_renderWindow(0),
    m_viewport(0),
    m_cameraObject(0),
    m_QmlUI(0)
{
    setAutoBufferSwap(false);
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::SampleBuffers));
    setAttribute(Qt::WA_OpaquePaintEvent);
    resize(1024, 768);

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
    delete m_cameraObject;
}

void OgreWidget::paintGL()
{
    // Render Ogre
    Ogre::WindowEventUtilities::messagePump();
    m_root->renderOneFrame();

    // Set a clear pass to give the renderer a clear renderstate
    static Ogre::Pass* clearPass = 0;
    if (!clearPass)
    {
        Ogre::MaterialPtr clearMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");
        clearPass = clearMat->getTechnique(0)->getPass(0);
    }
    m_sceneManager->_setPass(clearPass, true, false);

    // Render the QDeclarativeView texture on top
    glBindTexture(GL_TEXTURE_2D, m_QmlUI->textureId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float top = (rect().center().y() - m_QmlUI->pos().y()) / float(rect().center().y());
    float left = (rect().center().x() - m_QmlUI->pos().x()) / float(-rect().center().x());
    float bottom = (rect().center().y() - m_QmlUI->pos().y() - m_QmlUI->height()) / float(rect().center().y());
    float right = (rect().center().x() - m_QmlUI->pos().x() - m_QmlUI->width()) / float(-rect().center().x());

    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(left, top, -1.0f); // Top Left
    glTexCoord2f(0.0, 1.0); glVertex3f(left, bottom, -1.0f); // Bottom Left
    glTexCoord2f(1.0, 1.0); glVertex3f(right, bottom, -1.0f); // Bottom Right
    glTexCoord2f(1.0, 0.0); glVertex3f(right, top, -1.0f); // Top Right
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
}

void OgreWidget::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e)
    makeCurrent();
    paintGL();
    swapBuffers();
}

void OgreWidget::initializeGL()
{
    initOgre();

    m_QmlUI = new DeclarativeViewTexture(this);
    m_QmlUI->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_QmlUI->setSource(QUrl(QString(appPath() + "/resources/example.qml")));
    m_QmlUI->rootContext()->setContextProperty("Camera", m_cameraObject);
    m_QmlUI->rootContext()->setContextProperty("Window", this);
}

void OgreWidget::resizeGL(int w, int h)
{
    if (m_renderWindow) {
        m_renderWindow->resize(w, h);
        m_renderWindow->windowMovedOrResized();
    }
    if (m_camera) {
        Ogre::Real aspectRatio = Ogre::Real(w) / Ogre::Real(h);
        m_camera->setAspectRatio(aspectRatio);
    }
    if (m_QmlUI) {
        m_QmlUI->resize(w, h);
    }
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

    params["externalGLControl"] = "true";
    params["currentGLContext"] = "true";

    //Finally create our window.
    m_renderWindow = m_root->createRenderWindow("OgreWindow", width(), height(), false, &params);
    m_renderWindow->setVisible(true);

    // Load resources
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(QString(appPath() + "/resources/data.zip").toAscii().data(), "Zip");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Setup scene
    m_sceneManager = m_root->createSceneManager(Ogre::ST_GENERIC, "mySceneManager");
    m_camera = m_sceneManager->createCamera("myCamera");
    m_camera->setNearClipDistance(1);
    m_camera->setFarClipDistance(99999);
    m_viewport = m_renderWindow->addViewport(m_camera);
    m_viewport->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
    m_viewport->setClearEveryFrame(true);
    m_camera->setAspectRatio(Ogre::Real(width()) / Ogre::Real(height()));

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
}
