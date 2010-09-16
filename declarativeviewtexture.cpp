#include "declarativeviewtexture.h"

#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QPaintEvent>
#include <QtCore/QDebug>

static inline void qgl_byteSwapImage(QImage &img, GLenum pixel_type)
{
    const int width = img.width();
    const int height = img.height();

    if (pixel_type == GL_UNSIGNED_BYTE && QSysInfo::ByteOrder == QSysInfo::LittleEndian)
    {
        for (int i = 0; i < height; ++i) {
            uint *p = (uint *) img.scanLine(i);
            for (int x = 0; x < width; ++x)
                p[x] = ((p[x] << 16) & 0xff0000) | ((p[x] >> 16) & 0xff) | (p[x] & 0xff00ff00);
        }
    } else {
        for (int i = 0; i < height; ++i) {
            uint *p = (uint *) img.scanLine(i);
            for (int x = 0; x < width; ++x)
                p[x] = (p[x] << 8) | ((p[x] >> 24) & 0xff);
        }
    }
}

DeclarativeViewTexture::DeclarativeViewTexture(QWidget *parent) :
    QDeclarativeView(parent),
    m_bufferPainter(0)
{
    setAttribute(Qt::WA_DontShowOnScreen);
    setOptimizationFlag(QGraphicsView::IndirectPainting);

    glGenTextures(1, &m_textureId);
}

DeclarativeViewTexture::~DeclarativeViewTexture()
{

}

void DeclarativeViewTexture::paintEvent(QPaintEvent *event)
{
    // Render the view in an offscreen image
    delete m_bufferPainter;
    m_bufferPainter = new QPainter;
    QRegion exposedRegion = event->region();
    QImage im(exposedRegion.boundingRect().size(), QImage::Format_ARGB32_Premultiplied);
    im.fill(Qt::transparent);
    m_bufferPainter->begin(&im);
    m_bufferPainter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    m_bufferPainter->translate(-exposedRegion.boundingRect().topLeft());
    m_bufferPainter->setClipRegion(exposedRegion);
    QDeclarativeView::paintEvent(event);
    m_bufferPainter->end();

    // Upload the image in graphics memory
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    qgl_byteSwapImage(im, GL_UNSIGNED_BYTE);
    foreach (const QRect &rect, exposedRegion.rects()) {
        if (rect.size() == size()) {
            glTexImage2D(GL_TEXTURE_2D, 0, 4, rect.width(), rect.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, im.bits());
            break;
        } else {
            QRect adjustedRect = rect.translated(-exposedRegion.boundingRect().topLeft());
            QImage subIm = im.copy(adjustedRect);
            glTexSubImage2D(GL_TEXTURE_2D, 0, rect.left(), rect.top(), rect.width(), rect.height(), GL_RGBA, GL_UNSIGNED_BYTE, subIm.bits());
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DeclarativeViewTexture::drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[])
{
    QDeclarativeView::drawItems(m_bufferPainter, numItems, items, options);
}

void DeclarativeViewTexture::drawBackground(QPainter *painter, const QRectF &rect)
{
    QDeclarativeView::drawBackground(m_bufferPainter, rect);
}

void DeclarativeViewTexture::drawForeground(QPainter *painter, const QRectF &rect)
{
    QDeclarativeView::drawForeground(m_bufferPainter, rect);
}
