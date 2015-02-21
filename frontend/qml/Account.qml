import QtQuick 2.4
import "Components"
import "js/account.js" as Lib

Rectangle {
    id: account
    anchors.fill: parent
    clip: true
    color: "#ff777777"

    function show(account_id) {
        if( ! app.openAccount(account_id) ) {
            console.error("Unable to open account id#" + account_id)
            return false
        }

        var main_profile = Lib.getProfile(1)

        // Create main profile, if it doesn't exists
        if( main_profile.address === undefined ) {
            main_profile = {
                address: '',
                data: {first_name: 'FirstName', last_name: 'LastName', birth_date: '04.01.1988', },
                overlay: {},
                description: ''
            }
            main_profile.id = Lib.createProfile(main_profile)
        }

        account.visible = true
        profile.createObject(sheet, {obj_data: main_profile}).master()
        update()
    }
    function hide() {
        account.visible = false
    }
    function update() {
        console.log("Update")
        returnToBounds()
    }

    // View navigation functions
    function moveViewTo(target_point, duration_x, duration_y, easing_x, easing_y) {
        duration_x = duration_x !== undefined ? duration_x : 1000
        duration_y = duration_y !== undefined ? duration_y : 1000
        easing_x = easing_x !== undefined ? easing_x : Easing.OutExpo
        easing_y = easing_y !== undefined ? easing_y : Easing.OutExpo

        move_to.stop()

        move_to.target_point = target_point
        move_to.duration_x = duration_x
        move_to.duration_y = duration_y
        move_to.easing_x = easing_x
        move_to.easing_y = easing_y

        move_to.start()
    }

    function returnToBounds() {
        moveViewTo(Qt.point( Lib.getBoundX(sheet.x), Lib.getBoundY(sheet.y) ), 500, 500)
    }

    function moveToCenterOf(target_point) {
        moveViewTo(Qt.point( Lib.getBoundX(target_point.x), Lib.getBoundY(target_point.y) ), 200, 200)
    }

    property point _prev_pos // Used in inertia animation
    function activateInertia() {
        // TODO: rewrite to infinitie animation
        var new_x = sheet.x + (sheet.x - _prev_pos.x) * 20 * 100
        var new_y = sheet.y + (sheet.y - _prev_pos.y) * 20 * 100
        var bound_x = Lib.getBoundX(new_x)
        var bound_y = Lib.getBoundY(new_y)

        var duration_x = 100000
        var duration_y = 100000

        var easeing_x = Easing.OutExpo
        var easeing_y = Easing.OutExpo

        if( bound_x !== new_x ) {
            duration_x = Math.abs(bound_x / new_x) * duration_x
            easeing_x = Easing.OutBack
        }
        if( bound_y !== new_y ) {
            duration_y = Math.abs(bound_y / new_y) * duration_y
            easeing_y = Easing.OutBack
        }

        moveViewTo(Qt.point( bound_x, bound_y ), duration_x, duration_y, easeing_x, easeing_y)
    }

    Rectangle {
        id: visible_area
        anchors.fill: parent
        color: "#ccc"
        clip: true

        onWidthChanged: returnToBounds()
        onHeightChanged: returnToBounds()

        MouseArea {
            id: mouse_area
            anchors.fill: parent

            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

            property point _grab_point

            onClicked: {
                console.log("Clicked")
                if( mouse.button === Qt.RightButton ) {
                    moveToCenterOf(Qt.point(sheet.x - mouse.x + visible_area.width/2, sheet.y - mouse.y + visible_area.height/2))
                }
                mouse.accepted = false
            }

            onDoubleClicked: {
                console.log("Double clicked")
                mouse.accepted = false
            }

            onPressed: {
                move_to.stop()
                _grab_point = Qt.point(sheet.x - mouse.x, sheet.y - mouse.y)
                _prev_pos = Qt.point(sheet.x, sheet.y)
            }
            onReleased: {
                activateInertia()
            }

            onPositionChanged: {
                if( mouse.buttons & Qt.LeftButton == 1 ) {
                    // Set previous point to calculate speed of inertia
                    _prev_pos = Qt.point(sheet.x, sheet.y)

                    sheet.x = mouse.x + _grab_point.x
                    sheet.y = mouse.y + _grab_point.y
                }
            }

            onWheel: {
                if( wheel.angleDelta.y < 0 ) {
                    // Zoom out
                    sheet.scale /= 2
                } else {
                    // Zoom in
                    sheet.scale *= 2
                }
            }

            Rectangle {
                id: sheet

                width: childrenRect.width
                height: childrenRect.height
                onWidthChanged: returnToBounds()
                onHeightChanged: returnToBounds()

                property int scaledWidth: width * scale
                property int scaledHeight: height * scale
                property int scaledX: x + width/2 - (width * scale)/2
                property int scaledY: y + height/2 - (height * scale)/2

                Behavior on scale {
                    SequentialAnimation {
                        NumberAnimation { duration: 200; easing.type: Easing.OutCubic }
                        ScriptAction { script: returnToBounds() }
                    }
                }
            }
        }

        states: State {
            name: "ShowBars"
            when: mouse_area.pressed || inertia.running
            PropertyChanges { target: vertical_scrollbar; opacity: 1 }
            PropertyChanges { target: horizontal_scrollbar; opacity: 1 }
        }
        transitions: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; duration: 400 }
            NumberAnimation { property: "opacity"; from: 1.0; duration: 2000; easing.type: Easing.OutCubic }
        }

        SequentialAnimation{
            id: move_to
            property point target_point
            property int easing_x
            property int easing_y
            property int duration_x
            property int duration_y
            ParallelAnimation {
                PropertyAnimation{ target: sheet; property: "x"; to: move_to.target_point.x; duration: move_to.duration_x; easing.type: move_to.easing_x }
                PropertyAnimation{ target: sheet; property: "y"; to: move_to.target_point.y; duration: move_to.duration_y; easing.type: move_to.easing_y }
            }
        }
    }

    ScrollBar {
        id: horizontal_scrollbar
        width: visible_area.width-12; height: 12
        anchors.bottom: visible_area.bottom
        opacity: 0
        orientation: Qt.Horizontal
        position: - sheet.scaledX / sheet.scaledWidth
        pageSize: visible_area.width / sheet.scaledWidth
    }

    ScrollBar {
        id: vertical_scrollbar
        width: 12; height: visible_area.height-12
        anchors.right: visible_area.right
        opacity: 0
        orientation: Qt.Vertical
        position: - sheet.scaledY / sheet.scaledHeight
        pageSize: visible_area.height / sheet.scaledHeight
    }

    Component {
        id: profile
        Profile {}
    }
}

