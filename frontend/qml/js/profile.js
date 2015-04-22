function updateObjData() {
    obj_data.data.name = data_name.text
    _updateDate()
}

function _updateDate() {
    // Parse & simple check date
    var arrdata = data_birth_date.text.split("/")
    for( var i in arrdata ) {
        arrdata[i] = parseInt(arrdata[i])
        if( arrdata[i] < 1 ) {
            obj_data.data.birth_date = null
            return
        }
    }
    obj_data.data.birth_date = Date.UTC(arrdata[2], arrdata[1]-1, arrdata[0])/1000
}
