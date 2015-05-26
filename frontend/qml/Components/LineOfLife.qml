import QtQuick 2.4
import QtGraphicalEffects 1.0
import co.asocial 1.0 // WDate enums
import "LineOfLife"
import "../js/lineoflife.js" as L
import "../js/account.js" as A

Rectangle {
    id: lineoflife

    property var _profile_data

    property var _visible_from
    property var _visible_interval

    visible: false

    color: "#77ffffff"
    radius: 5
    border.width: 1

    clip: true

    function setProfile(profile_obj) {
        _profile_data = profile_obj.obj_data.data

        lineoflife.visible = true

        resetView()
    }

    function resetView() {
        _visible_from = _profile_data.birth_date !== null ? _profile_data.birth_date + wdate.tzOffsetSec() : 0
        _visible_interval = _profile_data.death_date !== null ? _profile_data.death_date + wdate.tzOffsetSec() - _visible_from : wdate.currentUnixtime() - _visible_from

        // Make small prepend & append to see the full picture
        var pend = Math.ceil(_visible_interval/20)
        _visible_from -= pend
        _visible_interval += pend*2

        L.setupAxis()
    }

    function newEvent(unixtime) {
        console.log("LineOfLife: Creating new event " + unixtime)
        console.log(JSON.stringify(_profile_data))
    }

    onWidthChanged: {
        L.setAxisDetailLevel()
        L.setupAxis()
    }

    on_Visible_intervalChanged: {
        L.setAxisDetailLevel()
    }

    Item {
        id: content
        anchors.fill: parent

        function zoom(unixtime) {
            if( L.current_detail_level === WDate.HOUR )
                return

            // Zoom half of current visible
            if( unixtime !== undefined )
                _visible_from = unixtime - Math.ceil(_visible_interval / 4)
            else
                _visible_from += Math.ceil(_visible_interval / 2)

            _visible_interval = Math.ceil(_visible_interval / 2)

            L.setupAxis()
            axis_cursor.setTo(unixtime)
        }

        function zoomOut(unixtime) {
            if( L.current_detail_level === WDate.YEAR && axis.children.length >= 100 )
                return

            // Zoom out twice of current visible
            if( unixtime !== undefined )
                _visible_from = unixtime - _visible_interval
            else
                _visible_from -= Math.ceil(_visible_interval / 2)
            _visible_interval *= 2

            L.setupAxis()
        }

        function centerTo(unixtime) {
            _visible_from = unixtime - Math.ceil(_visible_interval / 2)

            L.setupAxis()
        }

        Item {
            id: axis
            anchors.fill: parent

            Component {
                id: axis_mark
                AxisMark {}
            }
        }

        Item {
            id: events
            anchors.fill: parent

            Component {
                id: event_mark
                EventMark {}
            }
        }

        MouseArea {
            id: axis_mouse_area
            anchors.fill: parent

            hoverEnabled: true

            property var _prev_from
            property int _push_x

            property bool _stealed
            property point _push_point

            onPressed: {
                console.log("LineOfLife pressed")
                _prev_from = _visible_from
                _push_x = mouse.x

                _push_point = Qt.point(mouse.x, mouse.y)
                _stealed = false
                L.delayedActionStart(mouse, L.newEventPos)
            }

            onReleased: {
                console.log("LineOfLife released")
                L.delayedActionStop()
            }

            onMouseXChanged: {
                axis_cursor.setTo(L.pointToTime(mouse.x))

                if( pressed ) {
                    _visible_from = _prev_from - L.pointToTime(mouse.x - _push_x) + _visible_from

                    L.setupAxis()

                    // Release if mouse is far away from the last point
                    if( _stealed !== true ) {
                        if( Math.abs(_push_point.x - mouse.x) + Math.abs(_push_point.y - mouse.y) > 10 ) {
                            _stealed = true
                            L.delayedActionStop()
                        }
                    }
                }
            }

            onWheel: {
                console.log("LineOfLife wheel")

                var time = axis_cursor.unixtime

                if( wheel.angleDelta.y > 0 )
                    content.zoom(time)
                else if( wheel.angleDelta.y < 0 )
                    content.zoomOut()
                else
                    content.pos(time)
            }

            Item {
                id: active_content
                anchors.fill: parent
            }
        }

        AxisCursor {
            id: axis_cursor
            x: L.timeToPoint(unixtime)
            anchors.bottom: parent.bottom
            height: axis.height

            visible: axis_mouse_area.containsMouse
        }
    }

    WaitAction {
        id: delayed_action
    }
}
