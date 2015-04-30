var DETAIL = {
    "hour":  0,
    "day":   1,
    "month": 2,
    "year":  3
}

var _levels = [
     { seconds: 60*60,        minwidth: 15 * screenScale, size: 0.30, format: "hh",   }, // 0 hour
     { seconds: 60*60*24,     minwidth: 14 * screenScale, size: 0.40, format: "dd",   }, // 1 day
     { seconds: 60*60*24*31,  minwidth: 20 * screenScale, size: 0.60, format: "MM",   }, // 2 month
     { seconds: 60*60*24*365, minwidth: 20 * screenScale, size: 0.80, format: "yyyy", }  // 3 year
]

function getAxisDetailLevel() {
    var level = 0
    for( level in _levels ) {
        var delta = timeToPoint(lineoflife._from + _levels[level].seconds)
        console.log(delta + " min: " + _levels[level].minwidth)
        if( delta > _levels[level].minwidth )
            break
    }
    return parseInt(level) // Due to level is string right now
}

function genAxisMarks(detail, unixtime_from, unixtime_to) {
    var out = []
    var dp = new Date(unixtime_from*1000)

    // Reset time
    if( detail > DETAIL.month )
        dp.setMonth(0)
    if( detail > DETAIL.day )
        dp.setDate(1)
    if( detail > DETAIL.hour )
        dp.setHours(0)
    dp.setMinutes(0)
    dp.setSeconds(0)

    var ut = null

    console.log(detail)
    console.log(unixtime_from)
    console.log(unixtime_to)

    while( true ) {
        ut = dp.getTime() / 1000 | 0
        console.log(ut)

        if( detail === DETAIL.year )
            dp.setFullYear(dp.getFullYear()+1)
        if( detail === DETAIL.month )
            dp.setMonth(dp.getMonth()+1)
        if( detail === DETAIL.day )
            dp.setDate(dp.getDate()+1)
        if( detail === DETAIL.hour )
            dp.setHours(dp.getHours()+1)

        if ( ut < unixtime_from )
            continue // Skip dates before
        else if( ut > unixtime_to )
            break // Skip dates after

        out.push(ut)
    }

    return out
}

function createDateMark(unixtime) {
    var ts = new Date(unixtime*1000)

    // Determine level
    var level = DETAIL.hour
    if( ts.getHours() === 0 ) {
        level = DETAIL.day
        if( ts.getDate() === 1 ) {
            level = DETAIL.month
            if( ts.getMonth() === 0 )
                level = DETAIL.year
        }
    }

    return date_mark.createObject(
                axis_date_marks, {
                    x: timeToPoint(unixtime),
                    unixtime: unixtime,
                    format: _levels[level].format,
                    height: axis_date_marks.height * _levels[level].size
                })
}

function setupAxis() {
    if( lineoflife.visible === false || lineoflife._from === undefined || lineoflife._to === undefined )
        return

    for( var i in axis_date_marks.children )
        axis_date_marks.children[i].destroy()

    var v = visibleTime()

    var marks = genAxisMarks(getAxisDetailLevel(), v.from, v.to)

    for( i in marks ) {
        var obj = createDateMark(marks[i])
    }
}

function pointToTime(point) {
    return Math.ceil((point / axis.width) * (lineoflife._to - lineoflife._from) + lineoflife._from)
}

function timeToPoint(unixtime) {
    return Math.ceil(((unixtime - lineoflife._from) / (lineoflife._to - lineoflife._from)) * axis.width)
}

function visibleTime() {
    var segment = visibleSegment()
    return {
        from: pointToTime(segment.from),
        to: pointToTime(segment.to)
    }
}

function visibleSegment() {
    if( axis_background.width > visible_area.width ) {
        return {
            from: Math.max(0, visible_area.contentX - axis_background.anchors.leftMargin),
            to: Math.min(visible_area.contentX - axis_background.anchors.leftMargin + visible_area.width, axis_background.width)
        }
    } else
        return { from: 0, to: axis_background.width }
}
