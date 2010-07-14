import Qt 4.7

Item {
    id: item1
    width: 1024
    height: 768
    clip: true

    Rectangle {
        id: rectangle1
        y: 193
        width: 135
        height: 412
        color: "#64ffffff"
        radius: 20
        anchors.left: parent.left
        anchors.leftMargin: 28
        anchors.verticalCenter: parent.verticalCenter
        border.width: 3
        border.color: "#e6161616"
        clip: true

        Image {
            id: image1
            height: 40
            opacity: 0.4
            anchors.top: parent.top
            anchors.topMargin: 14
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            fillMode: "PreserveAspectFit"
            smooth: true
            source: "logo.png"
        }
    }
}
