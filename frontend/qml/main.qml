import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true
    width: 360
    height: 360

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }

    Component.onCompleted: {
        console.log("Connecting listener of password requests")
        app.requestPassword.connect(password_request.show)
        password_request.done.connect(app.responsePassword)
    }

    PasswordRequest {
        id: password_request
    }
}
