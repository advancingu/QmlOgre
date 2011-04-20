#include "ogreitem.h"
#include "ogrenode.h"
#include "cameranodeobject.h"

#include <QtCore/QPropertyAnimation>

OgreItem::OgreItem(QSGItem *parent)
    : QSGItem(parent)
    , m_timerID(0)
    , m_fakeAnim(this, "")
{
    setFlag(ItemHasContents);

    // Hack to get continuous updates
    m_fakeAnim.setDuration(10000);
    m_fakeAnim.setStartValue(0);
    m_fakeAnim.setEndValue(1);
    m_fakeAnim.setLoopCount(-1);
    m_fakeAnim.start();
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
