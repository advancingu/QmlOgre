/*!
 * \copyright (c) Nokia Corporation and/or its subsidiary(-ies) (qt-info@nokia.com) and/or contributors
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * \license{This source file is part of QmlOgre abd subject to the BSD license that is bundled
 * with this source code in the file LICENSE.}
 */

#ifndef EXAMPLEAPP_H
#define EXAMPLEAPP_H

#include "../lib/ogreengine.h"
#include "cameranodeobject.h"

#include <QtQuick/QQuickView>

class ExampleApp : public QQuickView
{
    Q_OBJECT
public:
    explicit ExampleApp(QWindow *parent = 0);
    ~ExampleApp();
    
signals:
    void ogreInitialized();

public slots:
    void initializeOgre();
    void addContent();
    
private:
    CameraNodeObject *m_cameraObject;
    OgreEngine *m_ogreEngine;

    Ogre::SceneManager *m_sceneManager;
    Ogre::Camera *m_camera;
    Ogre::Root *m_root;
};

#endif // EXAMPLEAPP_H
