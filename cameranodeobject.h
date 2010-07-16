#ifndef CAMERANODEOBJECT_H
#define CAMERANODEOBJECT_H

#include <QObject>

namespace Ogre {
class SceneNode;
class Camera;
}

class CameraNodeObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal yaw READ yaw WRITE setYaw)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom)
public:
    explicit CameraNodeObject(Ogre::Camera *cam, QObject *parent = 0);

    Ogre::SceneNode *sceneNode() const
    { return m_node; }
    Ogre::Camera *camera() const
    { return m_camera; }

    qreal yaw() const
    { return m_yaw; }
    qreal pitch() const
    { return m_pitch; }
    qreal zoom() const
    { return m_zoom; }
    void setYaw(qreal y)
    { m_yaw = y; updateRotation(); }
    void setPitch(qreal p)
    { m_pitch = p; updateRotation(); }
    void setZoom(qreal z);

private:
    void updateRotation();

    Ogre::SceneNode *m_node;
    Ogre::Camera *m_camera;

    qreal m_yaw;
    qreal m_pitch;
    qreal m_zoom;
};

#endif // CAMERANODEOBJECT_H
