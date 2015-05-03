function updateObjData() {
    obj_data.data.name = data_name.text
    _updateDate()
}

function _updateDate() {
    // Parse & simple check date
    if( wdate.checkFormat(data_birth_date.text, "dd/MM/yyyy") )
        obj_data.data.birth_date = wdate.unixtimeFromString(data_birth_date.text, "dd/MM/yyyy")
}
