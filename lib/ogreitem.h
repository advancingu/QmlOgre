/*!
 * \copyright (c) Nokia Corporation and/or its subsidiary(-ies) (qt-info@nokia.com) and/or contributors
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * \license{This source file is part of QmlOgre abd subject to the BSD license that is bundled
 * with this source code in the file LICENSE.}
 */

#ifndef OGREITEM_H
#define OGREITEM_H

#include "ogreengine.h"
#include "ogrecamerawrapper.h"

#include <QtQuick/QQuickItem>
#include <QtCore/QPropertyAnimation>

class OgreItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QObject *camera READ camera WRITE setCamera)
    Q_PROPERTY(OgreEngine *ogreEngine READ ogreEngine WRITE setOgreEngine)

public:
    OgreItem(QQuickItem *parent = 0);

    QObject *camera() const { return dynamic_cast<QObject *>(m_camera); }
    void setCamera(QObject *camera);

    OgreEngine *ogreEngine() const { return m_ogreEngineItem; }
    void setOgreEngine(OgreEngine *ogreEngine);

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    void timerEvent(QTimerEvent *);

private:
    int m_timerID;

    OgreCameraWrapper *m_camera;
    OgreEngine *m_ogreEngineItem;
};

#endif // OGREITEM_H
