/***************************************************************************
**
** This file is part of the qmlogre example on http://qt.gitorious.org.
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).*
**
** All rights reserved.
** Contact:  Nokia Corporation (qt-info@nokia.com)
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**  * Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
**  * Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with ** the distribution.
**  * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor the
**    names of its contributors may be used to endorse or promote products
**    derived from this software without specific ** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**************************************************************************/

import QtQuick 2.0
import Ogre 1.0

Rectangle {
    id: ogre
    width: 1024
    height: 768
    color: "black"

    Image {
        id: back
        anchors.fill: parent
        source: "qrc:/images/GrassandSky.png"
        Behavior on opacity { NumberAnimation { } }
    }


    OgreItem {
        id: ogreitem
        width: 600; height: 400
        anchors.left: toolbar1.left
        anchors.leftMargin: -5
        anchors.top: toolbar1.bottom
        anchors.topMargin: 6
        camera: Camera
        ogreEngine: OgreEngine

        Behavior on opacity { NumberAnimation { } }
        Behavior on width { NumberAnimation { } }
        Behavior on height { NumberAnimation { } }

        states: [
            State {
                name: "State1"

                PropertyChanges {
                    target: ogreitem
                    width: ogre.width
                    height: ogre.height
                }
                PropertyChanges {
                    target: toolbar1
                    x: 5
                    y: -toolbar1.height - 6
                }

                PropertyChanges {
                    target: toolbar4
                    anchors.top: ogreitem.top
                    anchors.topMargin: 5
                }
                PropertyChanges {
                    target: toolbar3
                    anchors.top: ogreitem.top
                    anchors.topMargin: 5
                }
                PropertyChanges {
                    target: back
                    opacity: 0
                }
            }
        ]

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton

            property int prevX: -1
            property int prevY: -1

            onPositionChanged: {
                if (pressedButtons & Qt.LeftButton) {
                    if (prevX > -1)
                        ogreitem.camera.yaw -= (mouse.x - prevX) / 2
                    if (prevY > -1)
                        ogreitem.camera.pitch -= (mouse.y - prevY) / 2
                    prevX = mouse.x
                    prevY = mouse.y
                }
                if (pressedButtons & Qt.RightButton) {
                    if (prevY > -1)
                        ogreitem.camera.zoom = Math.min(6, Math.max(0.1, ogreitem.camera.zoom - (mouse.y - prevY) / 100));
                    prevY = mouse.y
                }
            }
            onReleased: { prevX = -1; prevY = -1 }
        }
    }

    Rectangle {
        id: toolbar1
        x: 200
        y: 200
        width: 25
        height: 25
        radius: 5
        Behavior on x { NumberAnimation { } }
        Behavior on y { NumberAnimation { } }

        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }

        border.width: 2
        border.color: "#1a1a1a"

        Image {
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent
            smooth: true
            fillMode: "Stretch"
            source: "qrc:/images/move.gif"
        }

        MouseArea {
            anchors.fill: parent
            drag.target: toolbar1
            drag.axis: "XandYAxis"
            drag.minimumX: 0
            drag.minimumY: 0
            drag.maximumX: ogre.width - toolbar1.width
            drag.maximumY: ogre.height - toolbar1.height
        }
    }

    Rectangle {
        id: toolbar2
        width: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.left: toolbar1.right
        anchors.leftMargin: 6
        anchors.top: toolbar1.top
        anchors.bottom: toolbar1.bottom
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent

            onClicked: ogreitem.opacity = ogreitem.opacity == 1 ? 0 : 1
        }

        Rectangle {
            id: toolbar22
            x: 0
            y: -2
            radius: 12
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#5a5a5a"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
            rotation: -35
            anchors.rightMargin: 6
            anchors.bottomMargin: 6
            anchors.leftMargin: 6
            anchors.topMargin: 6
            anchors.fill: parent
        }
        border.width: 2
    }

    Rectangle {
        id: toolbar4
        width: 25
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.top: toolbar1.top
        anchors.right: toolbar3.left
        anchors.rightMargin: 6
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            onClicked: { ogreitem.smooth = !ogreitem.smooth }
        }

        Text {
            anchors.fill: parent
            text: "AA"
            font.bold: true
            font.pixelSize: parent.height * 0.55
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

            Rectangle {
                height: parent.height
                width: 2
                anchors.centerIn: parent
                color: "#BB1111"
                rotation: 40
                visible: !ogreitem.smooth
            }
         }
        border.width: 2
    }

    Rectangle {
        id: toolbar3
        width: 25
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.top: toolbar1.top
        anchors.right: ogreitem.right
        anchors.rightMargin: 5
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            onClicked: { ogreitem.state = ogreitem.state == '' ? 'State1' : '' }
        }

        Rectangle {
            id: toolbar31
            color: "#28ffffff"
            radius: 2
            border.width: 2
            border.color: "#000000"
            anchors.rightMargin: 7
            anchors.leftMargin: 7
            anchors.topMargin: 7
            anchors.bottomMargin: 7
            anchors.fill: parent

            Rectangle {
                id: toolbar311
                height: 3
                color: "#000000"
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
            }
        }
        border.width: 2
    }

    Item {
        id: camerawrapper
        property real yaw: 0
        property real pitch: 0
        property real zoom: 1

        onYawChanged: ogreitem.camera.yaw = yaw
        onPitchChanged: ogreitem.camera.pitch = pitch
        onZoomChanged: ogreitem.camera.zoom = zoom

        Behavior on yaw { NumberAnimation{ } }
        Behavior on pitch { NumberAnimation{ } }
        Behavior on zoom { NumberAnimation{ } }
    }

    Rectangle {
        id: rectangle1
        width: 139
        height: 208
        radius: 15
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#6f6f6f"
            }

            GradientStop {
                position: 0.24
                color: "#141414"
            }

            GradientStop {
                position: 1
                color: "#50000000"
            }
        }
        anchors.left: rectangle2.left
        anchors.leftMargin: -5
        anchors.top: rectangle2.bottom
        anchors.topMargin: 6
        border.width: 4
        border.color: "#1a1a1a"
        clip: false

        Behavior on opacity { PropertyAnimation { } }

        Image {
            id: image1
            width: 135
            height: 31
            anchors.top: parent.top
            anchors.topMargin: 9
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            fillMode: "PreserveAspectFit"
            smooth: true
            source: "qrc:/images/qt-logo.png"
        }

        Item {
            id: rectangle3
            x: 89
            y: 95
            width: 30
            height: 30
            anchors.horizontalCenterOffset: 35
            anchors.centerIn: image7

            MouseArea {
                id: mouse_area1
                anchors.fill: parent

                onPressed: camerawrapper.yaw += 20
            }

            Image {
                id: image5
                rotation: -90
                smooth: true
                anchors.fill: parent
                source: "qrc:/images/arrow.png"
            }
        }

        Item {
            id: rectangle4
            x: 27
            y: 95
            width: 30
            height: 30
            anchors.horizontalCenterOffset: -35
            anchors.centerIn: image7
            MouseArea {
                id: mouse_area2
                anchors.fill: parent

                onPressed: camerawrapper.yaw -= 20
            }

            Image {
                id: image4
                rotation: 90
                smooth: true
                anchors.fill: parent
                source: "qrc:/images/arrow.png"
            }
        }

        Item {
            id: rectangle5
            y: 64
            width: 30
            height: 30
            anchors.verticalCenterOffset: -35
            anchors.centerIn: image7
            MouseArea {
                id: mouse_area3
                anchors.fill: parent

                onPressed: camerawrapper.pitch -= 20
            }

            Image {
                id: image3
                rotation: 180
                smooth: true
                anchors.fill: parent
                source: "qrc:/images/arrow.png"
            }
        }

        Item {
            id: rectangle6
            x: 58
            y: 124
            width: 30
            height: 30
            anchors.verticalCenterOffset: 35
            anchors.centerIn: image7
            anchors.horizontalCenterOffset: 0
            MouseArea {
                id: mouse_area4
                x: 0
                y: -1
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                anchors.fill: parent

                onPressed: camerawrapper.pitch += 20
            }

            Image {
                id: image6
                smooth: true
                anchors.fill: parent
                source: "qrc:/images/arrow.png"
            }
        }

        Image {
            id: image7
            x: 84
            y: 0
            width: 24
            height: 24
            anchors.verticalCenterOffset: 9
            anchors.centerIn: parent
            smooth: true
            source: "qrc:/images/circle.png"

            MouseArea {
                id: mouse_area5
                x: 0
                y: -1
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onClicked: { camerawrapper.yaw = 0; camerawrapper.pitch = 0 }
            }
        }

        Image {
            id: image8
            x: 20
            y: 182
            width: 30
            height: 30
            anchors.bottomMargin: 9
            anchors.bottom: parent.bottom
            anchors.horizontalCenterOffset: 14
            anchors.horizontalCenter: rectangle4.horizontalCenter
            smooth: true
            source: "qrc:/images/minus.png"

            MouseArea {
                id: mouse_area6
                x: 0
                y: -1
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onPressed: camerawrapper.zoom /= 1.3
            }
        }

        Image {
            id: image9
            width: 30
            height: 30
            anchors.horizontalCenterOffset: -14
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 9
            anchors.horizontalCenter: rectangle3.horizontalCenter
            smooth: true
            source: "qrc:/images/plus.png"

            MouseArea {
                id: mouse_area7
                x: 0
                y: 0
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onPressed: camerawrapper.zoom *= 1.3
            }
        }
    }

    Rectangle {
        id: rectangle2
        x: 31
        y: 269
        width: 25
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }

        border.width: 2
        border.color: "#1a1a1a"

        Image {
            id: image2
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent
            smooth: true
            fillMode: "Stretch"
            source: "qrc:/images/move.gif"
        }

        MouseArea {
            anchors.fill: parent
            drag.target: rectangle2
            drag.axis: "XandYAxis"
            drag.minimumX: 0
            drag.minimumY: 0
            drag.maximumX: ogre.width - rectangle2.width
            drag.maximumY: ogre.height - rectangle2.height
        }
    }

    Rectangle {
        id: rectangle10
        width: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.left: rectangle2.right
        anchors.leftMargin: 6
        anchors.top: rectangle2.top
        anchors.bottom: rectangle2.bottom
        border.color: "#1a1a1a"

        MouseArea {
            anchors.fill: parent
            drag.minimumY: 0
            drag.axis: "XandYAxis"
            drag.minimumX: 0
            drag.target: rectangle10
            drag.maximumY: ogre.height - rectangle10.height
            drag.maximumX: ogre.width - rectangle10.width

            onClicked: ogre.state = ogre.state == '' ? 'State1' : ''
        }

        Rectangle {
            id: rectangle11
            x: 0
            y: -2
            radius: 12
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#5a5a5a"
                }

                GradientStop {
                    position: 1
                    color: "#000000"
                }
            }
            rotation: -35
            anchors.rightMargin: 6
            anchors.bottomMargin: 6
            anchors.leftMargin: 6
            anchors.topMargin: 6
            anchors.fill: parent
        }
        border.width: 2
    }

    Rectangle {
        id: rectangle12
        width: 25
        height: 25
        radius: 5
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                position: 1
                color: "#c8919191"
            }
        }
        anchors.top: rectangle2.top
        anchors.right: rectangle1.right
        anchors.rightMargin: 5
        border.color: "#1a1a1a"

        MouseArea {
            property bool fullscreen: false
            anchors.fill: parent
            onClicked: {
                if (fullscreen)
                    Window.showNormal();
                else
                    Window.showFullScreen();
                fullscreen = !fullscreen;
            }
        }

        Rectangle {
            id: rectangle13
            color: "#28ffffff"
            radius: 2
            border.width: 2
            border.color: "#000000"
            anchors.rightMargin: 7
            anchors.leftMargin: 7
            anchors.topMargin: 7
            anchors.bottomMargin: 7
            anchors.fill: parent

            Rectangle {
                id: rectangle14
                height: 3
                color: "#000000"
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: parent.top
            }
        }
        border.width: 2
    }
    states: [
        State {
            name: "State1"

            PropertyChanges {
                target: rectangle1
                opacity: 0
            }
        }
    ]
}
