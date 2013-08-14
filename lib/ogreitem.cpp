/*!
 * \copyright (c) Nokia Corporation and/or its subsidiary(-ies) (qt-info@nokia.com) and/or contributors
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * \license{This source file is part of QmlOgre abd subject to the BSD license that is bundled
 * with this source code in the file LICENSE.}
 */

#include "ogreitem.h"
#include "ogrenode.h"

OgreItem::OgreItem(QQuickItem *parent)
    : QQuickItem(parent)
    , m_timerID(0)
    , m_camera(0)
    , m_ogreEngineItem(0)
{
    setFlag(ItemHasContents);
    setSmooth(false);

    startTimer(16);
}

QSGNode *OgreItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (width() <= 0 || height() <= 0 || !m_camera || !m_ogreEngineItem) {
        delete oldNode;
        return 0;
    }

    OgreNode *node = static_cast<OgreNode *>(oldNode);
    if (!node)
    {
        node = new OgreNode();
        node->setOgreEngineItem(m_ogreEngineItem);
        node->setCamera(m_camera->camera());
    }

    node->setSize(QSize(width(), height()));
    node->update();
    // mark texture dirty, otherwise Qt will not trigger a redraw (preprocess())
    node->markDirty(QSGNode::DirtyMaterial);

    return node;
}

void OgreItem::timerEvent(QTimerEvent *)
{
    update();
}

void OgreItem::setCamera(QObject *camera)
{
    m_camera = qobject_cast<OgreCameraWrapper*>(camera);
}

void OgreItem::setOgreEngine(OgreEngine *ogreEngine)
{
    m_ogreEngineItem = ogreEngine;
}
