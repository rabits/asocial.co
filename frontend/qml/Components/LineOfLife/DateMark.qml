import QtQuick 2.4
import "../../js/lineoflife.js" as L

Rectangle {
    id: root

    anchors.verticalCenter: parent.verticalCenter
    width: 1
    height: axis.height * 0.5
    color: "#000000"

    property string format: "dd/MM/yyyy"
    property int unixtime

    Text {
        id: cursor_text
        anchors {
            left: parent.right
            bottom: parent.bottom
            margins: 1
        }

        text: Qt.formatDateTime(new Date((unixtime !== undefined ? unixtime : L.pointToTime(root.x)) * 1000), root.format)

        font.pixelSize: root.height / 4
        font.family: "monospaced"
    }
}
