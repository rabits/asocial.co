import QtQuick 2.4
import "Components"
import "js/userinteraction.js" as U

Item {
    id: root

    property alias delayed_action: delayed_action

    WaitAction {
        id: delayed_action
    }

    Component.onCompleted: {
        U.init(app, root)
    }
}

