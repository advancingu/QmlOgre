QmlOgre
=======

Integrates Ogre3D into a QML scene by rendering Ogre to a FBO.

* Window states, the event loop and user input are managed by Qt
* Uses a single OpenGL context created by Qt that is shared with Ogre
* Ogre renders into a texture via FBO
* The texture is applied to a simple rectangular QML item
* Easily permits QML items in front of and behind the item showing the Ogre scene
