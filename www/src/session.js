"use strict";

var Session = {
    user: 0,
    token: "",
    startdate:  DATE_MIN,
    enddate:    DATE_MIN,
    startloc:   [-10.000, 160.000],
    paths: {
    }
};

function request_new_path()
{
    var msg = {
        cmd:        "new_path",
        user:       Session.user,
        token:      Session.token,
        startdate:  date2array(Session.startdate),
        enddate:    date2array(Session.enddate),
        startloc:   Session.startloc,
    };
    //console.log(msg);
	ws.send(CBOR.encode(msg));
}