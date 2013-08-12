#include "exampleapp.h"

#include "../lib/ogreitem.h"
#include "../lib/ogreengineitem.h"

#include <QCoreApplication>
#include <QtQml/QQmlContext>
#include <QDir>

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

ExampleApp::ExampleApp(QWindow *parent) :
    QQuickView(parent)
  , m_cameraObject(0)
  , m_ogreEngineItem(0)
  , m_sceneManager(0)
  , m_camera(0)
{
    qmlRegisterType<OgreItem>("Ogre", 1, 0, "OgreItem");
}

ExampleApp::~ExampleApp()
{
    if (m_sceneManager) {
        m_ogreEngineItem->m_root->destroySceneManager(m_sceneManager);
    }
}

void ExampleApp::addContent()
{
    m_ogreEngineItem = new OgreEngineItem();
    m_ogreEngineItem->setQuickWindow(this);

    // Load resources
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(QString(appPath() + "/resources/data.zip").toLatin1().data(), "Zip");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Setup scene
    m_sceneManager = m_ogreEngineItem->m_root->createSceneManager(Ogre::ST_GENERIC, "mySceneManager");
    m_camera = m_sceneManager->createCamera("myCamera");
    m_camera->setNearClipDistance(1);
    m_camera->setFarClipDistance(99999);
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


    // set up QML scene
    setResizeMode(QQuickView::SizeRootObjectToView);
    setSource(QUrl::fromLocalFile("resources/example.qml"));
    rootContext()->setContextProperty("Window", this);
    rootContext()->setContextProperty("Camera", m_cameraObject);
}
