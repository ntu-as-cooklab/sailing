"use strict";

var session = {
    user: 0,
    token: "",
    paths: {
    }
};

function request_path()
{
	var startdate = new Date(document.getElementById("startdate").value);
	var startloc = orig.getLatLng();
	ws.send(CBOR.encode({
		cmd: "newPath",
		date: [startdate.getYear(),startdate.getMonth(),startdate.getDay(),startdate.getHours()],
		orig: [startloc.lat, startloc.lng],
	}));
}