import QtQuick 2.4
import co.asocial 1.0

Rectangle {
    id: root

    anchors.verticalCenter: parent.verticalCenter
    width: 1
    height: axis.height * 0.5
    color: "#00f"

    property string format: "dd/MM/yyyy"
    property var unixtime
    property color textColor: app.getTextColor(color)

    Rectangle {
        id: text_background

        anchors {
            horizontalCenter: parent.left
            top: parent.bottom
        }

        radius: 2 * screenScale
        color: Qt.rgba(root.color.r, root.color.g, root.color.b, 0.8)

        width: mark_text.contentWidth
        height: mark_text.contentHeight

        Text {
            id: mark_text

            text: wdate.format(unixtime*1000, root.format)

            font.pixelSize: 12
            font.family: "monospaced"
            color: textColor
        }
    }
}
