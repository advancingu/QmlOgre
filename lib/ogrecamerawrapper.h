#ifndef OGRECAMERAWRAPPER_H
#define OGRECAMERAWRAPPER_H

#include <QObject>
#include <Ogre.h>

class OgreCameraWrapper : public QObject
{
    Q_OBJECT

public:
    OgreCameraWrapper(QObject *parent = 0);
    virtual ~OgreCameraWrapper() {}
    Ogre::Camera* camera() { return m_camera; }

protected:
    Ogre::Camera *m_camera;
};

#endif // OGRECAMERAWRAPPER_H
