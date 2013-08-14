QmlOgre
=======
QmlOgre is a library that allows integrating Ogre3D into Qt QML scenes.

Features
--------

* Renders Ogre viewports to FBO (Frame Buffer Objects) which are then applied to QML items as textures
* Window states, the application event loop and user input are managed by Qt
* Creates an OpenGL context for Ogre which is shared with Qt's QML OpenGL context
* Allows using the full range of QML features
* Places no restriction on the depth sorting of Ogre items versus other QML elements

Documentation
-------------

The code is split into the ```lib``` library code and ```example``` application code.

License
-------
QmlOgre is licensed under the BSD license. See ```LICENSE``` for details.
