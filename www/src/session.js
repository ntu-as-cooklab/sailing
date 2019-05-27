"use strict";

var Session = {
    user: 0,
    token: "",
    startdate: DATE_MIN,
    enddate: DATE_MIN,
    paths: {
    }
};

function request_path()
{
	var startdate = new Date(document.getElementById("startdate").value);
	var startloc = orig.getLatLng();
	ws.send(CBOR.encode({
        cmd:    "newPath",
        user:   Session.user,
        token:  Session.token,
		date:   [startdate.getYear(),startdate.getMonth(),startdate.getDay(),startdate.getHours()],
        orig:   [startloc.lat, startloc.lng],
	}));
}