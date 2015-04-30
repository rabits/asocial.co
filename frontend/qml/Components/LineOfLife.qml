import QtQuick 2.4
import QtGraphicalEffects 1.0
import "LineOfLife"
import "../js/lineoflife.js" as L

// TODO: rewrite to PinchArea due to rectangle width limit
// TODO: use QDateTime due to limitations of minimal date (1900-01-01) of javascript Date

Rectangle {
    id: lineoflife

    property var _profile_data
    property int _from
    property int _to
    property var _test_var

    visible: false

    color: "#77ffffff"
    radius: 5
    border.width: 1

    clip: true

    function setProfile(profile_obj) {
        _profile_data = profile_obj.obj_data.data
        console.log((new Date()).getTimezoneOffset())
        _from = _profile_data.birth_date ? _profile_data.birth_date + (new Date()).getTimezoneOffset() * 60 : 0
        _to = (new Date()).getTime()/1000
        lineoflife.visible = true
        L.setupAxis()
    }

    onWidthChanged: L.setupAxis()

    Flickable {
        id: visible_area
        anchors.fill: parent
        pressDelay: 300

        contentWidth: content.width
        contentHeight: content.height

        clip: true

        MouseArea {
            id: mouse_area
            parent: visible_area
            anchors.fill: parent
            z: -1

            acceptedButtons: Qt.LeftButton | Qt.MiddleButton | Qt.RightButton

            onPressed: {
                console.log("Pressed LineOfLife")
            }
            onReleased: {
                console.log("Released LineOfLife")
            }

            onWheel: {
                console.log("Wheel LineOfLife")
                var unixtime_pos = L.pointToTime(axis_cursor.x)
                if( content.zoom(wheel.angleDelta.y, unixtime_pos) )
                    axis_cursor.setTo(L.timeToPoint(unixtime_pos), unixtime_pos)
            }
        }

        Item {
            id: content
            height: lineoflife.height
            width: lineoflife.width

            /*Behavior on width {
                SequentialAnimation {
                    NumberAnimation { duration: 1000; easing.type: Easing.OutExpo }
                }
            }*/

            property int _target_zoom: 0
            function zoom(delta, unixtime_target) {
                if( delta < 0 && _target_zoom > 0 ) {
                    _target_zoom--
                } else if( delta > 0 && L.getAxisDetailLevel() > 0 ) {
                    _target_zoom++
                } else
                    return false

                content.width = lineoflife.width * Math.pow(2, _target_zoom)

                // Set position to center of the target
                if( unixtime_target !== undefined ) {
                    visible_area.contentX = Math.min(Math.max(0, L.timeToPoint(unixtime_target) - visible_area.width/2), content.width - visible_area.width)
                }

                visible_area.returnToBounds()

                L.setupAxis()

                return true
            }

            Item {
                id: axis_background
                anchors {
                    fill: parent
                    leftMargin: 20
                    rightMargin: 20
                }
                LinearGradient {
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        topMargin: parent.height/2 - height/2
                    }

                    height: parent.height/20

                    start: Qt.point(0, 0)
                    end: Qt.point(width, 0)

                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#ff000000" }
                        GradientStop { position: 0.95; color: "#ff000000" }
                        GradientStop { position: 1.0; color: "#00000000" }
                    }
                }

                MouseArea {
                    id: axis_mouse_area
                    anchors.fill: parent
                    hoverEnabled: true

                    onPressed: {
                        console.log("Pressed Axis")
                        L.setupAxis()
                    }
                    onReleased: {
                        console.log("Released Axis")
                    }

                    onMouseXChanged: {
                        axis_cursor.setTo(mouse.x, L.pointToTime(mouse.x))
                    }

                    AxisCursor {
                        id: axis_cursor
                        anchors.bottom: parent.bottom
                        height: axis.height

                        visible: axis_mouse_area.containsMouse
                    }
                }

                Item {
                    id: axis
                    anchors {
                        fill: parent
                        rightMargin: axis_background.width * 0.05
                    }

                    Item {
                        id: axis_date_marks
                        anchors.fill: parent

                        Component {
                            id: date_mark
                            DateMark {}
                        }
                    }
                }
            }
        }
    }

    ScrollBar {
        id: horizontal_scrollbar
        width: visible_area.width; height: 8
        anchors.bottom: parent.bottom
        opacity: 0.1
        visible: content.width > visible_area.width
        orientation: Qt.Horizontal
        position: visible_area.visibleArea.xPosition
        pageSize: visible_area.width / content.width

        states: State {
            name: "visible"
            when: visible_area.moving
            PropertyChanges { target: horizontal_scrollbar; opacity: 1.0 }
        }
        transitions: Transition {
            NumberAnimation { property: "opacity"; duration: 1000; easing.type: Easing.OutCubic }
        }
    }
}
