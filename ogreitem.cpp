#include "ogreitem.h"
#include "ogrenode.h"
#include "cameranodeobject.h"

OgreItem::OgreItem(QSGItem *parent)
    : QSGItem(parent)
    , m_timerID(0)
{
    setFlag(ItemHasContents);
    setTargetFPS(60);
}

void OgreItem::setTargetFPS(int fps)
{
    if (fps == m_targetFPS)
        return;

    m_targetFPS = fps;

    if (m_timerID)
        killTimer(m_timerID);
    //m_timerID = startTimer(1000 / m_targetFPS);

    emit targetFPSChanged();
}

void OgreItem::timerEvent(QTimerEvent *e)
{
    if (e->timerId() != m_timerID)
        return;

    update();
}

QSGNode *OgreItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (width() <= 0 || height() <= 0) {
        delete oldNode;
        return 0;
    }

    OgreNode *node = static_cast<OgreNode *>(oldNode);
    if (!node)
        node = new OgreNode();

    node->setSize(QSize(width(), height()));
    node->setAAEnabled(smooth());
    node->update();

    m_camera = static_cast<QObject *>(node->camera());

    return node;
}
