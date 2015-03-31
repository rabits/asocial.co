import QtQuick 2.4
import "Components"
import "js/account.js" as A

Rectangle {
    id: account
    anchors.fill: parent
    clip: true
    color: "#ff777777"

    property Profile _main_profile

    function show(account_id) {
        if( ! app.openAccount(account_id) ) {
            console.error("Unable to open account id#" + account_id)
            return false
        }

        var main_profile = A.getProfile(1)

        // Create main profile, if it doesn't exists
        if( main_profile.address === undefined ) {
            main_profile = A.emptyProfileData()
            main_profile.address = A.createAddress()
            main_profile.id = A.createProfile(main_profile)
        }

        account.visible = true
        _main_profile = A.createProfileObj(main_profile)
        _main_profile.master()

        updateSheet()
    }
    function hide() {
        account.visible = false
    }

    function updateSheet() {
        console.log("Update sheet")

        // Find minimal x & y values and move sheet childrens
        var minX = null
        var minY = null
        for( var c in sheet.children ) {
            if( minX === null ) {
                minX = sheet.children[c].x
                minY = sheet.children[c].y
            } else {
                minX = minX > sheet.children[c].x ? sheet.children[c].x : minX
                minY = minY > sheet.children[c].y ? sheet.children[c].y : minY
            }
        }
        if( minX != 0 || minY != 0 ) {
            for( var c in sheet.children ) {
                sheet.children[c].x -= minX
                sheet.children[c].y -= minY
            }
        }

        A.returnToBounds()
    }

    // View navigation functions
    function moveToCenterOf(target_point, scale) {
        A.moveViewTo(Qt.point( A.getBoundX(target_point.x), A.getBoundY(target_point.y) ), 2000, 2000, Easing.OutExpo, Easing.OutExpo, scale, 2000, Easing.OutExpo)
    }

    property point _prev_pos // Used in inertia animation
    function activateInertia() {
        // TODO: rewrite to infinitie animation
        var new_x = sheet.x + (sheet.x - _prev_pos.x) * 20 * 100
        var new_y = sheet.y + (sheet.y - _prev_pos.y) * 20 * 100
        var bound_x = A.getBoundX(new_x)
        var bound_y = A.getBoundY(new_y)

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

        A.moveViewTo(Qt.point( bound_x, bound_y ), duration_x, duration_y, easeing_x, easeing_y)
    }

    Rectangle {
        id: visible_area
        anchors.fill: parent
        color: "#ccc"
        clip: true

        onWidthChanged: A.returnToBounds()
        onHeightChanged: A.returnToBounds()

        MouseArea {
            id: mouse_area
            anchors.fill: parent

            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

            property point _grab_point
            property bool _stealed: true
            property point _push_point

            onClicked: {
                console.log("Clicked Account")
                if( mouse.button === Qt.RightButton ) {
                    moveToCenterOf(Qt.point(sheet.x - mouse.x + visible_area.width/2, sheet.y - mouse.y + visible_area.height/2), sheet.scale)
                }
            }

            onPressed: {
                console.log("Pressed Account")
                move_to.stop()
                _push_point = Qt.point(mouse.x, mouse.y)
                _stealed = false
                _grab_point = Qt.point(sheet.x - mouse.x, sheet.y - mouse.y)
                _prev_pos = Qt.point(sheet.x, sheet.y)

                A.delayedActionStart(mouse, A.createNewProfileObj)
            }
            onReleased: {
                console.log("Released Account")
                activateInertia()
                A.delayedActionStop()
            }

            onPositionChanged: {
                if( mouse.buttons & Qt.LeftButton == 1 ) {
                    // Set previous point to calculate speed of inertia
                    _prev_pos = Qt.point(sheet.x, sheet.y)

                    sheet.x = mouse.x + _grab_point.x
                    sheet.y = mouse.y + _grab_point.y

                    // Release childrens if mouse is far away from the last point
                    if( _stealed !== true ) {
                        if( Math.abs(_push_point.x - mouse.x) + Math.abs(_push_point.y - mouse.y) > 10 ) {
                            _stealed = true
                            A.delayedActionStop()
                        }
                    }
                }
            }

            onWheel: {
                console.log("Wheel Account")
                _push_point = Qt.point(wheel.x, wheel.y)

                var scale_factor = ( wheel.angleDelta.y > 0 ) ? 2 : 1/2

                sheet.x = sheet.x - (_push_point.x - visible_area.width/2) * scale_factor
                sheet.y = sheet.y - (_push_point.y - visible_area.height/2) * scale_factor

                sheet.scale *= scale_factor
            }

            Rectangle {
                id: sheet

                width: childrenRect.width
                height: childrenRect.height
                onWidthChanged: A.returnToBounds()
                onHeightChanged: A.returnToBounds()

                property int scaledWidth: width * scale
                property int scaledHeight: height * scale
                property int scaledX: x + width/2 - (width * scale)/2
                property int scaledY: y + height/2 - (height * scale)/2
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

        SequentialAnimation {
            id: move_to
            property point target_point
            property double scale
            property int easing_x
            property int easing_y
            property int easing_scale
            property int duration_x
            property int duration_y
            property int duration_scale

            ParallelAnimation {
                PropertyAnimation { target: sheet; property: "x"; to: move_to.target_point.x; duration: move_to.duration_x; easing.type: move_to.easing_x }
                PropertyAnimation { target: sheet; property: "y"; to: move_to.target_point.y; duration: move_to.duration_y; easing.type: move_to.easing_y }
                PropertyAnimation { target: sheet; property: "scale"; to: move_to.scale; duration: move_to.duration_scale; easing.type: move_to.easing_scale }
            }
            //ScriptAction { script: A.returnToBounds() }
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

    WaitAction {
        id: delayed_action
    }

    Component {
        id: profile
        Profile {}
    }
}

