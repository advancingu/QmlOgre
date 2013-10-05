/*!
 * \copyright (c) Nokia Corporation and/or its subsidiary(-ies) (qt-info@nokia.com) and/or contributors
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * \license{This source file is part of QmlOgre abd subject to the BSD license that is bundled
 * with this source code in the file LICENSE.}
 */

#include "cameranodeobject.h"

#include <OgreRoot.h>
#include <OgreSceneNode.h>
#include <OgreCamera.h>

#include <QDebug>

static const Ogre::Vector3 initialPosition(0, 0, 300);

CameraNodeObject::CameraNodeObject(QObject *parent) :
    QObject(parent),
    OgreCameraWrapper(),
    m_camera(0),
    m_yaw(0),
    m_pitch(0),
    m_zoom(1)
{
    Ogre::SceneManager *sceneManager = Ogre::Root::getSingleton().getSceneManager("mySceneManager");

    // let's use the current memory address to create a unique name
    QString instanceName;
    instanceName.sprintf("camera_%08p", this);

    Ogre::Camera *camera = sceneManager->createCamera(instanceName.toLatin1().data());
    camera->setNearClipDistance(1);
    camera->setFarClipDistance(99999);
    camera->setAspectRatio(1);
    camera->setAutoTracking(true, sceneManager->getRootSceneNode());

    m_camera = camera;
    m_node = sceneManager->getRootSceneNode()->createChildSceneNode();
    m_node->attachObject(camera);
    camera->move(initialPosition);
}

void CameraNodeObject::updateRotation()
{
    m_node->resetOrientation();
    m_node->yaw(Ogre::Radian(Ogre::Degree(m_yaw)));
    m_node->pitch(Ogre::Radian(Ogre::Degree(m_pitch)));
}

void CameraNodeObject::setZoom(qreal z)
{
    m_zoom = z;
    m_node->resetOrientation();
    m_camera->setPosition(initialPosition * (1 / m_zoom));
    updateRotation();
}
