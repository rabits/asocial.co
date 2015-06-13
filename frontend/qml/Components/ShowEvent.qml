import QtQuick 2.4

Item {
    id: root
    anchors.fill: parent
    visible: false

    property var _event
    property var _unixtime
    property var _save_data_func

    function show(events, unixtime, save_data_func) {
        root.visible = true

        _unixtime = unixtime
        _event = events[_unixtime.toString()]

        _save_data_func = save_data_func

        editable_text.text = _event.text
    }

    function save() {
        _event.text = editable_text.text
        _save_data_func()
        // TODO: event unixtime change
    }

    function hide() {
        root.save()
        root.visible = false
        _event = null
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
            margins: 20 * screenScale
        }
        radius: 10 * screenScale

        EditableText {
            id: editable_text
            anchors {
                fill: parent
                margins: 10 * screenScale
            }
        }
    }
}
