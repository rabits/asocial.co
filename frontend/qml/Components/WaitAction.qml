import QtQuick 2.4

Rectangle {
    id: root
    visible: false
    opacity: 0.0

    property alias script: arrow_script.script

    function start(pos) {
        root.x = pos.x - root.radius
        root.y = pos.y - root.radius
        root.visible = true
        arrow_animation.start()
    }

    function stop() {
        arrow_animation.stop()
        root.visible = false
    }

    width: 50
    height: width
    radius: width/2
    border.width: 1

    Rectangle {
        id: arrow
        anchors.centerIn: parent
        width: 2
        height: parent.radius
        color: "#000"
        transform: Translate { y: -arrow.height/2 }
        transformOrigin: Item.Bottom
        SequentialAnimation on rotation {
            id: arrow_animation
            running: false
            RotationAnimation { from: 0; to: 360; duration: 1000 }
            ScriptAction { id: arrow_script }
            ScriptAction { script: root.stop() }
        }
    }

    states: [
        State {
            name: "visible"
            when: visible
            PropertyChanges { target: root; opacity: 1.0 }
        }
    ]
    transitions: Transition {
        NumberAnimation { property: "opacity"; duration: 500; from: 0.0; to: 1.0; easing.type: Easing.InQuart }
    }
}

