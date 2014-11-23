import QtQuick 2.3
import QtQuick.Layouts 1.1
import "Components"

Rectangle {
    id: component
    anchors.fill: parent
    color: "#cc000000"
    visible: false

    signal done(string password)

    property bool _create_new_password: false

    function show(description, create) {
        desc.text = description
        _create_new_password = create
        component.visible = true
        input.focus = true
    }

    function hideOk() {
        if( input.text == "" ) {
            desc.color = "#f00"
            desc.text = "Password can't be empty"
            return
        }
        if( _create_new_password ) {
            if( input.text != input_repeat.text ) {
                desc.color = "#f00"
                desc.text = "Passwords not match"
                input_repeat.text = ""
                input_repeat.focus = true
                return
            }
        }

        component.visible = false
        desc.text = "no description"
        done(input.text)
        input.text = ""
        input_repeat.text = ""
    }

    Rectangle {
        id: window
        color: "#fff"
        anchors.centerIn: parent
        border.color: "#000"
        border.width: 2 * screenScale
        radius: 5 * screenScale

        width: 300
        height: 100

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                id: desc
                text: "no description"
            }

            Rectangle {
                color: "#33000000"
                border.color: "#000"
                border.width: 2 * screenScale
                height: 30
                anchors {
                    left: parent.left
                    right: parent.right
                }

                Text {
                    color: "#44333333"
                    anchors.fill: parent

                    clip: true
                    visible: input.text == ""
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    font {
                        family: "monospace"
                        pixelSize: 20
                    }

                    text: "Enter password"
                }

                TextInput {
                    id: input
                    color: "#333"
                    anchors.fill: parent

                    clip: true
                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                    onAccepted: {
                        if( _create_new_password && input_repeat.text == "" )
                            input_repeat.focus = true
                        else
                            hideOk()
                    }

                    font {
                        bold: true
                        family: "monospace"
                        pixelSize: 20
                    }

                    passwordCharacter: "*"
                    echoMode: TextInput.Password
                }
            }

            Rectangle {
                color: "#33000000"
                border.color: input_repeat.text == input.text ? "#000" : "#f00"
                border.width: 2 * screenScale
                height: 30
                anchors.left: parent.left
                anchors.right: parent.right

                visible: _create_new_password

                Text {
                    color: "#44333333"
                    anchors.fill: parent

                    clip: true
                    visible: input_repeat.text == ""
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    font {
                        family: "monospace"
                        pixelSize: 20
                    }

                    text: "Repeat password"
                }

                TextInput {
                    id: input_repeat
                    color: "#333"
                    anchors.fill: parent

                    clip: true
                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                    onAccepted: hideOk()

                    font {
                        bold: true
                        family: "monospace"
                        pixelSize: 20
                    }

                    passwordCharacter: "*"
                    echoMode: TextInput.Password
                }
            }

            Button {
                id: button_ok
                anchors.horizontalCenter: parent.horizontalCenter

                width: 80
                height: 30
                color: "#595"

                caption: qsTr("Ok")

                onClicked: hideOk()
            }
        }

    }
}
