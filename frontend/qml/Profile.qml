import QtQuick 2.4
import "Components"

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

        border.width: (data_avatar_url.source == "") ? 1 : 0

        radius: 100
        color: "#dcffffff"
    }

    Rectangle {
        id: avatar_background
        anchors {
            top: parent.top
            topMargin: 50
            horizontalCenter: parent.horizontalCenter
        }

        width: 100
        height: 100
        border.width: 1

        color: "#ccc"

        Image {
            id: data_avatar_url
            anchors.fill: parent

            fillMode: Image.PreserveAspectFit

            source: obj_data.data.avatar_thumbnail_url
        }

        Text {
            anchors.fill: parent
            anchors.margins: 2

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            fontSizeMode: Text.Fit
            minimumPixelSize: 10
            font.pixelSize: 72
            font.weight: Font.Bold
            color: "#22000000"

            visible: data_avatar_url.source == ""

            text: qsTr("No Avatar")
        }
    }

    EditableText {
        id: data_first_name
        anchors {
            bottom: data_last_name.top
            horizontalCenter: parent.horizontalCenter
        }

        font.weight: Font.Bold
        font.pixelSize: 16
        style: Text.Outline
        styleColor: "#fff"

        default_text: qsTr("First Name")
        enabled: false

        text: obj_data.data.first_name
    }

    EditableText {
        id: data_last_name
        anchors {
            bottom: parent.bottom
            bottomMargin: 50
            horizontalCenter: parent.horizontalCenter
        }

        font.weight: Font.Bold
        font.pixelSize: 16
        style: Text.Outline
        styleColor: "#fff"

        default_text: qsTr("Last Name")
        enabled: false

        text: obj_data.data.last_name
    }

    EditableText {
        id: data_birth_date
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: avatar_background.bottom

        default_text: qsTr("Birth Date")
        enabled: false

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
            PropertyChanges { target: profile; width: 300; height: 400 }
            PropertyChanges { target: avatar_background; width: 200; height: 300; anchors.topMargin: 20 }
            PropertyChanges { target: data_avatar_url; source: obj_data.data.avatar_url }
            PropertyChanges { target: data_first_name; font.pixelSize: 20 }
            PropertyChanges { target: data_birth_date; visible: false }
            PropertyChanges { target: data_last_name; font.pixelSize: 20; anchors.bottomMargin: 20 }
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

