function getBoundX(x) {
    if( sheet.scaledWidth > visible_area.width ) {
        var leftbound = (sheet.scaledWidth - sheet.width)/2
        if( x > leftbound )
            return leftbound
        var rightbound = visible_area.width - (sheet.width + sheet.scaledWidth)/2
        if( x < rightbound )
            return rightbound
    } else
        return (visible_area.width - sheet.width)/2

    return x
}
function getBoundY(y) {
    if( sheet.scaledHeight > visible_area.height ) {
        var topbound = (sheet.scaledHeight - sheet.height)/2
        if( y > topbound )
            return topbound
        var bottombound = visible_area.height - (sheet.height + sheet.scaledHeight)/2
        if( y < bottombound )
            return bottombound
    } else
        return (visible_area.height - sheet.height)/2

    return y
}

var db = null

function setDB() {
    if( db === null )
        db = app.getCurrentAccount()
}

function getProfile(id_address) {
    setDB()
    return db.getProfile(id_address)
}

function createProfile(obj_data) {
    setDB()
    return db.createProfile(obj_data)
}
