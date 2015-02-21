import QtQuick 2.4

Item {
    id: profile
    objectName: "Profile"

    width: 200
    height: 200

    property var obj_data

    function master() {
        console.log("Set master profile")
        state = 'master'

        console.log(JSON.stringify(obj_data))
    }

    Rectangle {
        id: background
        anchors.fill: parent

        border.width: 1

        radius: 100
        color: "#dcffffff"
    }

    Text {
        anchors.fill: parent

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: obj_data.address
    }

    Text {
        anchors.fill: parent

        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop

        text: obj_data.data.first_name
    }

    Text {
        anchors.fill: parent

        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignTop

        text: obj_data.data.last_name
    }

    Text {
        anchors.fill: parent

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignBottom

        text: obj_data.data.birth_date
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true

        acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

        onClicked: {
            mouse.accepted = false
            console.log("clicked Profile")
        }
        onPressed: {
            mouse.accepted = false
            console.log("pushed Profile")
            //profile.state = profile.state === '' ? 'master' : ''
        }
        onDoubleClicked: {
            console.log("Doubleclicked Profile")
            profile.state = profile.state === '' ? 'master' : ''
        }
    }

    states: [
        State {
            name: "master"
            PropertyChanges { target: background; radius: 0 }
            PropertyChanges { target: profile; width: 300 }
            PropertyChanges { target: profile; height: 400 }
        }
    ]
    transitions: [
        Transition {
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation { property: 'radius'; duration: 500; easing.type: Easing.OutCubic }
                    NumberAnimation { properties: 'width,height'; duration: 500; easing.type: Easing.OutCubic }
                }
            }
        }
    ]
}

