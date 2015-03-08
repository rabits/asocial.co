import QtQuick 2.4
import "Components"
import "js/account.js" as A
import "js/profile.js" as P

Item {
    id: root
    objectName: "Profile"

    width: 200
    height: 200

    property var obj_data

    function master() {
        console.log("Set master profile")
        state = 'master'

        console.log(JSON.stringify(obj_data))
    }

    function mouseReleased(mouse) {
        console.log("Released Profile")
        background_highlight.visible = false
        profile_edit.stop()
    }

    Rectangle {
        id: background
        anchors.fill: parent
        antialiasing: true

        clip: true

        border.width: 1
        border.color: (obj_data.address === "") ? "#aaa" : "#000"

        radius: background.width/2
        color: (obj_data.address === "") ? "#33ffffff" : "#dcffffff"

        Rectangle {
            id: background_highlight
            anchors.fill: parent
            visible: false
            opacity: 0.2

            radius: parent.radius

            color: "#0f0"
        }
    }

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true

        acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

        onPressed: {
            mouse.accepted = false
            if( background.radius > 0 ) {
                // Background is not squared - so, let's check that point in radius
                if( Math.pow(mouse.x - root.x - background.radius, 2) + Math.pow(mouse.y - root.y - background.radius, 2) > Math.pow(background.radius, 2) )
                    return
            }

            console.log("Pressed Profile")
            background_highlight.visible = true
            profile_edit.start(mouse)
        }

        Rectangle {
            id: avatar_background
            antialiasing: true
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

                source: obj_data.data.avatar_url_eq
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
            next_item: data_last_name

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
            next_item: data_birth_date

            text: obj_data.data.last_name
        }

        EditableText {
            id: data_birth_date
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: avatar_background.bottom

            default_text: qsTr("Birth Date")
            enabled: false
            next_item: data_first_name

            text: obj_data.data.birth_date
        }

        WaitAction {
            id: profile_edit
            script: {
                if( root.state.split('_')[0] === "edit" ) {
                    root.state = root.state.split('_')[1]
                    P.updateData()
                    console.log(JSON.stringify(obj_data))
                    A.updateProfileData(obj_data)
                } else {
                    root.state = "edit_" + root.state
                }
            }
        }
    }

    states: [
        State {
            name: "master"
            PropertyChanges { target: background; radius: 0 }
            PropertyChanges { target: root; width: 300; height: 400 }
            PropertyChanges { target: avatar_background; width: 200; height: 300; anchors.topMargin: 20 }
            PropertyChanges { target: data_avatar_url; source: obj_data.data.avatar_url }
            PropertyChanges { target: data_first_name; font.pixelSize: 20 }
            PropertyChanges { target: data_birth_date; anchors.topMargin: 60; visible: false }
            PropertyChanges { target: data_last_name; font.pixelSize: 20; anchors.bottomMargin: 20 }
        },
        State {
            name: "edit_"
            PropertyChanges { target: data_first_name; enabled: true; editable: true }
            PropertyChanges { target: data_last_name; enabled: true; editable: true }
            PropertyChanges { target: data_birth_date; enabled: true; editable: true }
            PropertyChanges { target: background; color: "#0f0" }
        },
        State {
            name: "edit_master"
            extend: "master"
            PropertyChanges { target: data_first_name; enabled: true; editable: true }
            PropertyChanges { target: data_last_name; enabled: true; editable: true }
            PropertyChanges { target: data_birth_date; enabled: true; editable: true; visible: true }
            PropertyChanges { target: background; color: "#0f0" }
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

