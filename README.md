QmlOgre
=======

Integrates Ogre3D into a QML scene by rendering Ogre to a FBO.

* Window states, the event loop and user input are managed by Qt
* Uses a separate OpenGL context created by and shared with Qt's context into which Ogre renders
* Ogre renders into a texture via FBO
* The texture is applied to a simple rectangular QML item
* Easily permits QML items in front of and behind the item showing the Ogre scene
