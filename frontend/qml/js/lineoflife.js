var _levels = [
     { seconds: 60*60,        minwidth: 15 * screenScale, size: 0.30, format: "hh",   }, // 0 hour
     { seconds: 60*60*24,     minwidth: 14 * screenScale, size: 0.40, format: "dd",   }, // 1 day
     { seconds: 60*60*24*31,  minwidth: 20 * screenScale, size: 0.60, format: "MM",   }, // 2 month
     { seconds: 60*60*24*365, minwidth: 20 * screenScale, size: 0.80, format: "yyyy", }  // 3 year
]

function getAxisDetailLevel() {
    var level = 0
    for( level in _levels ) {
        var delta = timeToPoint(lineoflife._visible_from + _levels[level].seconds)
        if( delta > _levels[level].minwidth )
            break
    }
    return parseInt(level) // Due to level is string right now
}

function createDateMark(unixtime) {
    // Determine level of mark
    var level = wdate.getLevel(unixtime*1000)

    return date_mark.createObject(
                axis, {
                    x: timeToPoint(unixtime),
                    unixtime: unixtime,
                    format: _levels[level].format,
                    height: axis.height * _levels[level].size
                })
}

function setupAxis() {
    if( lineoflife.visible === false )
        return

    for( var i in axis.children )
        axis.children[i].destroy()

    var marks = wdate.getAxisMarks(getAxisDetailLevel(), lineoflife._visible_from*1000, lineoflife._visible_interval*1000)

    for( i in marks ) {
        var obj = createDateMark(marks[i])
    }
}

function pointToTime(point) {
    return Math.ceil((1.0 * point / axis_mouse_area.width) * lineoflife._visible_interval + lineoflife._visible_from)
}

function timeToPoint(unixtime) {
    return Math.ceil((1.0 * (unixtime - lineoflife._visible_from) / lineoflife._visible_interval) * axis_mouse_area.width)
}
