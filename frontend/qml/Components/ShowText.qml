import QtQuick 2.4

Item {
    id: root
    anchors.fill: parent
    visible: false

    function show(text) {
        root.visible = true
        editable_text.text = text
    }

    function hide() {
        root.visible = false
        editable_text.text = ""
    }

    MouseArea {
        anchors.fill: parent

        onClicked: root.hide()
    }

    Rectangle {
        id: show_text_background
        anchors {
            fill: parent
            margins: 20
        }
        radius: 10

        EditableText {
            id: editable_text
            anchors {
                fill: parent
                margins: 10
            }
        }
    }
}
