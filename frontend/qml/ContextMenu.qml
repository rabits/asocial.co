import QtQuick 2.4

Rectangle {
    id: component
    anchors.fill: parent
    color: "#cc000000"
    visible: false

    function show(items) {

    }

    Rectangle {
        id: window
        color: "#fff"
        anchors.centerIn: parent
        border.color: "#000"
        border.width: 2 * screenScale
        radius: 5 * screenScale

        ListView {
            id: list
            anchors.fill: parent
            anchors.margins: 5
            clip: true

            model: ListModel {}
            delegate: Rectangle {
                anchors.margins: 20
                height: 50
                width: ListView.view.width
                radius: 5
                color: "#22008800"

                Column {
                    anchors.fill: parent
                    Text {
                        text: name
                    }
                    Text {
                        text: description
                    }
                }
            }
            focus: true
            highlight: Rectangle {
                color: "#8800ff00"
            }
        }
    }
}

