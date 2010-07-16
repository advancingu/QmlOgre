import Qt 4.7

Item {
    id: item1
    width: 1024
    height: 768
    clip: true

    Rectangle {
        id: rectangle1
        width: 139
        height: 212
        radius: 15
        gradient: Gradient {
            GradientStop {
                id: gradientstop3
                position: 0
                color: "#6f6f6f"
            }

            GradientStop {
                id: gradientstop4
                position: 0.27
                color: "#141414"
            }

            GradientStop {
                id: gradientstop5
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
            opacity: 0.8
            anchors.top: parent.top
            anchors.topMargin: 9
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            fillMode: "PreserveAspectFit"
            smooth: true
            source: "logo.png"
        }

        Rectangle {
            id: rectangle3
            x: 89
            y: 95
            width: 24
            height: 26
            color: "#ffffff"

            MouseArea {
                id: mouse_area1
                anchors.fill: parent

                onPressed: Camera.yaw += 10
            }
        }

        Rectangle {
            id: rectangle4
            x: 27
            y: 95
            width: 24
            height: 26
            color: "#ffffff"
            MouseArea {
                id: mouse_area2
                anchors.fill: parent

                onPressed: Camera.yaw -= 10
            }
        }

        Rectangle {
            id: rectangle5
            x: 58
            y: 64
            width: 24
            height: 26
            color: "#ffffff"
            MouseArea {
                id: mouse_area3
                anchors.fill: parent

                onPressed: Camera.pitch -= 10
            }
        }

        Rectangle {
            id: rectangle6
            x: 58
            y: 126
            width: 24
            height: 26
            color: "#ffffff"
            MouseArea {
                id: mouse_area4
                x: 0
                y: -1
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                anchors.fill: parent

                onPressed: Camera.pitch += 10
            }
        }

        Rectangle {
            id: rectangle9
            x: 89
            y: 171
            width: 24
            height: 26
            color: "#ffffff"
            MouseArea {
                id: mouse_area7
                x: 0
                y: 0
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onPressed: Camera.zoom += 0.15
            }
        }

        Rectangle {
            id: rectangle7
            x: 58
            y: 95
            width: 24
            height: 26
            color: "#ffffff"
            MouseArea {
                id: mouse_area5
                x: 0
                y: -1
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onClicked: Camera.resetPosition()
            }
        }

        Rectangle {
            id: rectangle8
            x: 27
            y: 171
            width: 24
            height: 26
            color: "#ffffff"
            MouseArea {
                id: mouse_area6
                x: 0
                y: -1
                anchors.fill: parent
                anchors.topMargin: 0
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0

                onPressed: Camera.zoom -= 0.15
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
                id: gradientstop1
                position: 0
                color: "#c83e3e3e"
            }

            GradientStop {
                id: gradientstop2
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
