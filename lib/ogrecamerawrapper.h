/*!
 * \copyright (c) Nokia Corporation and/or its subsidiary(-ies) (qt-info@nokia.com) and/or contributors
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * \license{This source file is part of QmlOgre abd subject to the BSD license that is bundled
 * with this source code in the file LICENSE.}
 */

#ifndef OGRECAMERAWRAPPER_H
#define OGRECAMERAWRAPPER_H

#include <Ogre.h>

/**
 * @brief The OgreCameraWrapper class defines an interface
 * that allows assigning Ogre::Camera instances to OgreItem
 * via QML.
 *
 * Implementing classes must inherit from QObject.
 */
class OgreCameraWrapper
{
public:
    OgreCameraWrapper();
    virtual ~OgreCameraWrapper() {}
    virtual Ogre::Camera* camera() const = 0;
};

#endif // OGRECAMERAWRAPPER_H
