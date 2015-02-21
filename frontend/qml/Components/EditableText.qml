import QtQuick 2.4

Rectangle {
    id: editable_text

    color: "#00000000"

    width: text_content.childrenRect.width + 4
    height: text_content.childrenRect.height + 4

    radius: 2

    property alias font: bg_text.font
    property alias default_text: bg_text.text
    property alias text: text.text

    property Item next_item

    signal done(string text)

    function setEdit(is) {
        editable_text.enabled = true
        if( is === true ) {
            editable_text.state = 'edit'
        } else {
            editable_text.state = ''
        }
    }

    onFocusChanged: {
        if( focus === true )
            text.focus = true
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        cursorShape: Qt.IBeamCursor

        onEntered: {
            editable_text.border.width = 1
        }
        onExited: {
            editable_text.border.width = 0
        }

        Rectangle {
            id: text_content
            anchors {
                left: parent.left
                top: parent.top
                margins: 2
            }

            Text {
                id: bg_text
                color: '#aaa'
                visible: text.length === 0
            }
            TextInput {
                id: text
                selectByMouse: true
                enabled: false
                focus: editable_text.focus

                width: Math.max(contentWidth, bg_text.width)
                height: Math.max(contentHeight, bg_text.height)

                font: bg_text.font

                onAccepted: done(text.text)
                KeyNavigation.tab: next_item
            }
        }
    }

    states: [
        State {
            name: "edit"
            PropertyChanges { target: text; enabled: true }
            PropertyChanges { target: editable_text; color: '#ccffffff' }
        }
    ]
    transitions: [
        Transition {
            SequentialAnimation {
                ParallelAnimation {
                    ColorAnimation { duration: 500; easing.type: Easing.OutCubic }
                }
            }
        }
    ]
}

