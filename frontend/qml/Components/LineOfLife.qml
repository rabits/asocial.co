import QtQuick 2.4
import QtGraphicalEffects 1.0
import co.asocial 1.0 // WDate enums
import "LineOfLife"
import "../js/lineoflife.js" as L

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
        _visible_from = _profile_data.birth_date ? _profile_data.birth_date + wdate.tzOffsetSec() : 0
        _visible_interval = wdate.currentUnixtime() - _visible_from

        L.setupAxis()
    }

    onWidthChanged: L.setupAxis()

    Item {
        id: content
        anchors.fill: parent

        function zoom(unixtime) {
            if( L.getAxisDetailLevel() === WDate.HOUR )
                return

            // Zoom half of current visible
            if( unixtime !== undefined )
                _visible_from = unixtime - Math.ceil(_visible_interval / 4)
            else
                _visible_from += Math.ceil(_visible_interval / 2)

            _visible_interval = Math.ceil(_visible_interval / 2)

            L.setupAxis()
        }

        function zoomOut(unixtime) {
            if( L.getAxisDetailLevel() === WDate.YEAR && axis.children.length >= 100 )
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
                id: date_mark
                DateMark {}
            }
        }

        MouseArea {
            id: axis_mouse_area
            anchors.fill: parent

            hoverEnabled: true

            property var _prev_from
            property int _push_x

            onPressed: {
                console.log("LineOfLife pressed")
                _prev_from = _visible_from
                _push_x = mouse.x
            }

            onReleased: {
                console.log("LineOfLife released")
            }

            onMouseXChanged: {
                axis_cursor.setTo(mouse.x, L.pointToTime(mouse.x))

                if( pressed ) {
                    _visible_from = _prev_from - L.pointToTime(mouse.x - _push_x) + _visible_from

                    L.setupAxis()
                }
            }

            onWheel: {
                console.log("LineOfLife wheel")

                var time = L.pointToTime(wheel.x)

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
            anchors.bottom: parent.bottom
            height: axis.height

            visible: axis_mouse_area.containsMouse
        }
    }
}
