#ifndef OGREITEM_H
#define OGREITEM_H

#include <QSGItem>

class CameraNodeObject;

class OgreItem : public QSGItem
{
    Q_OBJECT

    Q_PROPERTY(int targetFPS READ targetFPS WRITE setTargetFPS NOTIFY targetFPSChanged)
    Q_PROPERTY(QObject *camera READ camera)

public:
    OgreItem(QSGItem *parent = 0);

    void setTargetFPS(int fps);
    int targetFPS() const { return m_targetFPS; }

    QObject *camera() const { return m_camera; }

Q_SIGNALS:
    void targetFPSChanged();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    void timerEvent(QTimerEvent *);

private:
    int m_targetFPS;

    int m_timerID;

    QObject *m_camera;
};

#endif // OGREITEM_H
