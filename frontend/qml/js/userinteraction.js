/**
 * UserInteraction library
 * Provides singleton user action functions
**/
.pragma library

var app = null
var _u = null

function init(app_obj, user_interaction_obj) {
    console.log("Init user interaction library")
    app = app_obj
    _u = user_interaction_obj
}

/**
 * Delayed Action
**/

function delayedActionStart(view_pos, slot) {
    delayedActionStop()
    return _u.delayed_action.start(view_pos, slot)
}

function delayedActionStop() {
    return _u.delayed_action.stop()
}
