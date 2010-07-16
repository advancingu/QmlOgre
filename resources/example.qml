import Qt 4.7

Item {
    id: item1
    width: 1024
    height: 768
    clip: true

    Item {
        id: camerawrapper
        property real yaw: 0
        property real pitch: 0
        property real zoom: 1

        onYawChanged: Camera.yaw = yaw
        onPitchChanged: Camera.pitch = pitch
        onZoomChanged: Camera.zoom = zoom

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
        z: 0
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
            source: "qt-logo.png"
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
                source: "arrow.png"
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
                source: "arrow.png"
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
                source: "arrow.png"
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
                source: "arrow.png"
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
            source: "circle.png"

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
            source: "minus.png"

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
            source: "plus.png"

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
        z: -1

        Image {
            id: image2
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent
            smooth: true
            fillMode: "Stretch"
            source: "move.gif"
        }

        MouseArea {
            anchors.fill: parent
            drag.target: rectangle2
            drag.axis: "XandYAxis"
            drag.minimumX: 0
            drag.minimumY: 0
            drag.maximumX: item1.width - rectangle2.width
            drag.maximumY: item1.height - rectangle2.height
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
            drag.maximumY: item1.height - rectangle10.height
            drag.maximumX: item1.width - rectangle10.width

            onClicked: item1.state = item1.state == '' ? 'State1' : ''
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
        z: -1
    }

    Rectangle {
        id: rectangle12
        x: 132
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
            anchors.fill: parent
            onClicked: Window.fullScreen ? Window.showNormal() : Window.showFullScreen()
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
        z: -1
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
