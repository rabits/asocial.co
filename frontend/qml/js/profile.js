function updateObjData() {
    obj_data.data.name = data_name.text
    var arrdata = data_birth_date.text.split("/")
    obj_data.data.birth_date = Date.UTC(arrdata[2], arrdata[1]-1, arrdata[0])/1000
}

