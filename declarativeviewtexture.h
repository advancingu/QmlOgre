#ifndef DECLARATIVEVIEWTEXTURE_H
#define DECLARATIVEVIEWTEXTURE_H

#include <QtDeclarative/QDeclarativeView>
#include <QtOpenGL/qgl.h>

class DeclarativeViewTexture : public QDeclarativeView
{
    Q_OBJECT
public:
    explicit DeclarativeViewTexture(QWidget *parent = 0);
    ~DeclarativeViewTexture();

    GLuint textureId() const
    { return m_textureId; }

protected:
    void paintEvent(QPaintEvent *event);
    void drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[]);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void drawForeground(QPainter *painter, const QRectF &rect);

private:
    QPainter *m_bufferPainter;
    GLuint m_textureId;
};

#endif // DECLARATIVEVIEWTEXTURE_H
