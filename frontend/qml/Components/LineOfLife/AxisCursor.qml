import QtQuick 2.4

Rectangle {
    id: root

    width: 1
    color: "#000000"

    function setTo(pos, unixtime) {
        root.x = pos
        root.text = Qt.formatDateTime(new Date(unixtime*1000), "dd/MM/yyyy hh:mm:ss")
    }

    property alias text: cursor_text.text

    Text {
        id: cursor_text
        anchors {
            left: parent.right
            bottom: parent.bottom
            margins: 1
        }

        font.pixelSize: root.height / 6
        font.family: "monospaced"

        states: State {
            name: "left"
            when: root.x + cursor_text.contentWidth - visible_area.contentX > visible_area.width - axis_background.anchors.leftMargin
            AnchorChanges {
                target: cursor_text
                anchors.left: undefined
                anchors.right: parent.right
                anchors.bottom: parent.bottom
            }
        }

        transitions: Transition {
            AnchorAnimation { duration: 200 }
        }
    }
}
