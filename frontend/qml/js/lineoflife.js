var _levels = [
     { seconds: 60*60,        minwidth: 6 * screenScale, size: 0.30, format: "hh",   }, // 0 hour
     { seconds: 60*60*24,     minwidth: 4 * screenScale, size: 0.40, format: "dd",   }, // 1 day
     { seconds: 60*60*24*31,  minwidth: 10 * screenScale, size: 0.60, format: "MM",   }, // 2 month
     { seconds: 60*60*24*365, minwidth: 20 * screenScale, size: 0.80, format: "yyyy", }  // 3 year
]

var current_detail_level = _levels.length - 1

function setAxisDetailLevel() {
    var level = null
    for( level in _levels ) {
        var delta = timeToPoint(lineoflife._visible_from + _levels[level].seconds)
        if( delta > _levels[level].minwidth )
            break
    }
    current_detail_level = parseInt(level) // Due to level is string right now
}

function createAxisMark(unixtime) {
    return axis_mark.createObject(axis, {})
}

function setMark(mark_obj, unixtime) {
    // Determine level of mark
    var level = wdate.getLevel(unixtime*1000)
    level = level < current_detail_level ? current_detail_level : level

    mark_obj.x = timeToPoint(unixtime)
    mark_obj.unixtime = unixtime
    mark_obj.format = _levels[level].format
    mark_obj.height = axis.height * _levels[level].size
    mark_obj.level = level
}

function createBirthDayMark(unixtime) {
    unixtime -= wdate.tzOffsetSec() // birth date is unixtime day without hours
    return event_mark.createObject(
                events, {
                    x: timeToPoint(unixtime),
                    unixtime: unixtime
                })
}

function createDeathDayMark(unixtime) {
    unixtime -= wdate.tzOffsetSec() // death date is unixtime day without hours
    return event_mark.createObject(
                events, {
                    x: timeToPoint(unixtime),
                    unixtime: unixtime
                })
}

function clean() {
    var i = 0

    for( i = events.children.length; i > 0 ; i-- )
        events.children[i-1].destroy()
}

function setupAxis() {
    if( lineoflife.visible === false )
        return

    clean()

    var marks = wdate.getAxisMarks(current_detail_level, lineoflife._visible_from*1000, lineoflife._visible_interval*1000)

    for( var i in marks ) {
        var obj = axis.children[i]

        if( obj === undefined )
            obj = createAxisMark()

        setMark(obj, marks[i])
        obj.init(axis.children[i-1])
    }

    // Cleaning not required axis marks
    for( i = axis.children.length; i > marks.length ; i-- )
        axis.children[i-1].destroy()

    if( _profile_data.birth_date !== null )
        createBirthDayMark(_profile_data.birth_date)
    if( _profile_data.death_date !== null )
        createDeathDayMark(_profile_data.death_date)
}

function pointToTime(point) {
    return Math.ceil((1.0 * point / axis_mouse_area.width) * lineoflife._visible_interval + lineoflife._visible_from)
}

function timeToPoint(unixtime) {
    return Math.ceil((1.0 * (unixtime - lineoflife._visible_from) / lineoflife._visible_interval) * axis_mouse_area.width)
}

function newEventPos(pos) {
    lineoflife.newEvent(L.pointToTime(pos.x))
}

/**
 * LineOfLife Delayed Action
**/

function delayedActionStart(view_pos, slot) {
    delayedActionStop()
    delayed_action.start(view_pos, slot)
}

function delayedActionStop() {
    delayed_action.stop()
}
