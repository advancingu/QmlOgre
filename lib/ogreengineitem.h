#ifndef OGREENGINEITEM_H
#define OGREENGINEITEM_H

#include <QQuickWindow>
#include <QOpenGLContext>
#include <Ogre.h>

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define OGRE_PLUGIN_DIR STRINGIFY(OGRE_PLUGIN_VAR)

namespace Ogre {
class Root;
class RenderTexture;
class Viewport;
class RenderTarget;
}

/**
 * @brief The OgreEngineItem class
 * Must only be constructed from within Qt QML rendering thread.
 */
class OgreEngineItem
{
public:
    OgreEngineItem(QQuickWindow *window);
    ~OgreEngineItem();
    void startEngine();


    void activateOgreState();
    void doneOgreState();

    QOpenGLContext* ogreContext() const;

    QSGTexture* createTextureFromId(uint id, const QSize &size, QQuickWindow::CreateTextureOptions options = QQuickWindow::CreateTextureOption(0)) const;

//private:
    Ogre::Root *m_root;
    Ogre::RenderWindow *m_ogreWindow;

    QQuickWindow *m_quickWindow;

    /** Pointer to QOpenGLContext to be used by Ogre. */
    QOpenGLContext* m_ogreContext;
    /** Pointer to QOpenGLContext to be restored after Ogre context. */
    QOpenGLContext* m_qtContext;

protected:
    void setQuickWindow(QQuickWindow *window);
};

#endif // OGREENGINEITEM_H
