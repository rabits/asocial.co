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

/**
 * DB Operations
**/

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

/**
 * Sheet Objects Operations
**/

function createProfileObj(profile_data) {
    console.log("Create profile object")

    var obj = _a.profile_component.createObject(_s, {obj_data: profile_data})

    _a.updateSheet()

    return obj
}
function createNewProfileObj(pos) {
    console.log("Create new profile object")

    var profile_id = createProfile(emptyProfileData())
    var obj = _a._master_profile.createConnection(profile_id, pos)

    return obj
}

function masterProfile() {
    return _a._master_profile
}

/**
 * Sheet Move Operations
**/

var _top_sheet_index = 0
function sheetItemTop(item) {
    item.z = ++_top_sheet_index
}

function convertViewPointToSheetPoint(point, scale) {
    scale = scale ? scale : _s.scale
    return {
        x: (point.x + _va.contentX) / scale,
        y: (point.y + _va.contentY) / scale
    }
}

function convertSheetPointToViewPoint(point, scale) {
    scale = scale ? scale : _s.scale
    return {
        x: point.x * scale - _va.contentX,
        y: point.y * scale - _va.contentY
    }
}

function sheetMoveTo(target_point, duration_x, duration_y) {
    _a.move_to.stop()

    _a.move_to.target_point = target_point
    _a.move_to.duration_x = duration_x ? duration_x : 1000
    _a.move_to.duration_y = duration_y ? duration_y : 1000

    _a.move_to.start()
}

function sheetCenterViewTo(point, duration_x, duration_y) {
    sheetMoveTo(Qt.point(_va.contentX + point.x - _va.width/2,
                         _va.contentY + point.y - _va.height/2),
                duration_x, duration_y)
}

function sheetScaleTo(vector, point) {
    var target_point = convertSheetPointToViewPoint(point, _s.targetScale((vector > 0 ) ? _s._target_scale + 1 : _s._target_scale - 1))
    sheetCenterViewTo(target_point)
}

/**
 * Account Delayed Action
**/

function delayedActionStart(view_pos, slot) {
    delayedActionStop()
    _a.delayed_action.start(view_pos, slot)
}

function delayedActionStop() {
    _a.delayed_action.stop()
}
