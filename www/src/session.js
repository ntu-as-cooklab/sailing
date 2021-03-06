"use strict";

const DATE_MIN = new Date(1979, 0,  1, 0);
const DATE_MAX = new Date(2011, 3, 31, 0);

var Session = {
    runId: 0,
    loginID: "guest",
    token: "",
    startdate:  DATE_MIN,
    enddate:    new Date(1979, 3, 1, 0),
    startloc:   [-10.000, 160.000],
	paths: [],
    color: '#FF0000',
    mode: 0,
    dataset: 0,
    destloc: [-14.000, 188.500],
    destheading: 0,
    altitude: 2,
    windlimit: 8,
    sailstarthour: 6,
    sailendhour: 18,
    alpha: 0.11,
};

var url = new URL(window.location.href);
Session.runId  = Number(url.searchParams.get("runId"));
Session.loginID  = url.searchParams.get("loginID");
if(!Session.loginID) Session.loginID = "guest";
Session.token = String(url.searchParams.get("token"));
console.log("runId:", Session.runId);
console.log("loginID:", Session.loginID);
console.log("token:", Session.token);

function session_restore()
{
    $("#loading").show();
    for (let i = 0; i < Session.paths.length; i++) {
        let path = Session.paths[i];
        if(map.hasLayer(path.polyline)) map.removeLayer(path.polyline);
        if(map.hasLayer(path.circles)) map.removeLayer(path.circles);
    }
    Session.paths = [];

	ws.send(CBOR.encode({cmd:"restore"}));
}