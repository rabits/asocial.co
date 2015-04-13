/*
 * Account library
 * Provides singleton account functions
**/
.pragma library

var app = null
var _a = null
var _s = null
var _va = null
var _db = null

function init(app_obj, account, sheet, visible_area) {
    console.log("Init account library")
    app = app_obj
    _a = account
    _s = sheet
    _va = visible_area
}

function initDB(account_id) {
    if( ! app.openAccount(account_id) ) {
        console.error("Unable to open account id#" + account_id)
        return false
    }
    _db = app.getCurrentAccount()
}

function moveViewTo(target_point, duration_x, duration_y, easing_x, easing_y, scale, duration_scale, easing_scale) {
    _a.move_to.stop()

    _a.move_to.target_point = target_point
    _a.move_to.duration_x = duration_x !== undefined ? duration_x : 1000
    _a.move_to.duration_y = duration_y !== undefined ? duration_y : 1000
    _a.move_to.easing_x = easing_x !== undefined ? easing_x : 22 // Easing.OutExpo
    _a.move_to.easing_y = easing_y !== undefined ? easing_y : 22 // Easing.OutExpo
    _a.move_to.scale = scale !== undefined ? scale : _s.scale
    _a.move_to.duration_scale = duration_scale !== undefined ? duration_scale : 1000
    _a.move_to.easing_scale = easing_scale !== undefined ? easing_scale : 22 // Easing.OutExpo

    _a.move_to.start()
}

function returnToBounds() {
    moveViewTo(Qt.point( getBoundX(_s.x), getBoundY(_s.y) ), 500, 500)
}

function getBoundX(x, mulscale) {
    mulscale = mulscale ? mulscale : 1
    if( _s.scaledWidth*mulscale > _va.width ) {
        var leftbound = - _s.scaledX*mulscale
        if( x > leftbound )
            return leftbound
        var rightbound = _va.width - _s.scaledX*mulscale - _s.scaledWidth*mulscale
        if( x < rightbound )
            return rightbound
    } else
        return _va.width/2 - (_s.scaledX*mulscale + _s.scaledWidth*mulscale/2)

    return x
}
function getBoundY(y, mulscale) {
    mulscale = mulscale ? mulscale : 1
    if( _s.scaledHeight*mulscale > _va.height ) {
        var topbound = - _s.scaledY*mulscale
        if( y > topbound )
            return topbound
        var bottombound = _va.height - _s.scaledY*mulscale - _s.scaledHeight*mulscale
        if( y < bottombound )
            return bottombound
    } else
        return _va.height/2 - (_s.scaledY*mulscale + _s.scaledHeight*mulscale/2)

    return y
}

function convertViewPointToSheetPoint(point, scale) {
    scale = scale ? scale : _s.scale
    return {
        x: (point.x - _s.x) / scale,
        y: (point.y - _s.y) / scale
    }
}

function convertSheetPointToViewPoint(point, scale) {
    scale = scale ? scale : _s.scale
    return {
        x: point.x * scale + _s.x,
        y: point.y * scale + _s.y
    }
}

function emptyProfileData() {
    return {
        id: null,
        address: '',
        data: {name: '', birth_date: null, avatar_url: '', avatar_url_eq: ''},
        overlay: {},
        description: ''
    }
}

function getProfile(id_address) {
    return _db.getProfile(id_address)
}

function createProfile(obj_data) {
    return _db.createProfile(obj_data)
}

function updateProfileData(profile) {
    return _db.updateProfileData(profile)
}

function createAddress() {
    return _db.createAddress()
}



function createProfileObj(profile_data) {
    console.log("Create profile object")

    var obj = _a.profile_component.createObject(_s, {obj_data: profile_data})

    _a.updateSheet()

    return obj
}
function createNewProfileObj(pos) {
    console.log("Create new profile object")

    pos = convertViewPointToSheetPoint(pos)
    var profile_id = createProfile(emptyProfileData())
    var obj = _a._master_profile.createConnection(profile_id, pos)

    return obj
}

function masterProfile() {
    return _a._master_profile
}

function delayedActionStart(pos, slot) {
    delayedActionStop()
    _a.delayed_action.start(pos, slot)
}

function delayedActionStop() {
    _a.delayed_action.stop()
}
