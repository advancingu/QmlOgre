#ifndef OGREITEM_H
#define OGREITEM_H

#include <QSGItem>
#include <QtCore/QPropertyAnimation>

class CameraNodeObject;

class OgreItem : public QSGItem
{
    Q_OBJECT

    Q_PROPERTY(QObject *camera READ camera)

public:
    OgreItem(QSGItem *parent = 0);

    QObject *camera() const { return m_camera; }

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
    int m_timerID;
    QPropertyAnimation m_fakeAnim;

    QObject *m_camera;
};

#endif // OGREITEM_H
