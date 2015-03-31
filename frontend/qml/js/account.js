function moveViewTo(target_point, duration_x, duration_y, easing_x, easing_y, scale, duration_scale, easing_scale) {
    move_to.stop()

    move_to.target_point = target_point
    move_to.duration_x = duration_x !== undefined ? duration_x : 1000
    move_to.duration_y = duration_y !== undefined ? duration_y : 1000
    move_to.easing_x = easing_x !== undefined ? easing_x : Easing.OutExpo
    move_to.easing_y = easing_y !== undefined ? easing_y : Easing.OutExpo
    move_to.scale = scale !== undefined ? scale : sheet.scale
    move_to.duration_scale = duration_scale !== undefined ? duration_scale : 1000
    move_to.easing_scale = easing_scale !== undefined ? easing_scale : Easing.OutExpo

    move_to.start()
}

function returnToBounds() {
    moveViewTo(Qt.point( A.getBoundX(sheet.x), A.getBoundY(sheet.y) ), 500, 500)
}

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

function convertPointToSheetCoord(point) {
    return {
        x: (point.x - sheet.scaledX) / sheet.scale,
        y: (point.y - sheet.scaledY) / sheet.scale
    }
}

function emptyProfileData() {
    return {
        id: null,
        address: '',
        data: {first_name: '', last_name: '', birth_date: '', avatar_url: '', avatar_url_eq: ''},
        overlay: {},
        description: ''
    }
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

function updateProfileData(profile) {
    setDB()
    return db.updateProfileData(profile)
}

function createAddress() {
    setDB()
    return db.createAddress()
}



function createProfileObj(profile_data) {
    console.log("Create profile object")

    var obj = profile.createObject(sheet, {obj_data: profile_data})

    updateSheet()

    return obj
}
function createNewProfileObj(pos) {
    console.log("Create new profile object")

    pos = convertPointToSheetCoord(pos)
    var profile_id = createProfile(emptyProfileData())
    var obj = account._main_profile.createConnection(profile_id, pos)

    return obj
}


function delayedActionStart(pos, slot) {
    delayedActionStop()
    delayed_action.start(pos, slot)
}

function delayedActionStop() {
    delayed_action.stop()
}
