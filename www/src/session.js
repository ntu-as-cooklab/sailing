"use strict";

var Session = {
    user: 0,
    token: "",
    startdate:  DATE_MIN,
    enddate:    new Date(1979, 1, 1, 0),
    startloc:   [-10.000, 160.000],
    paths: {
    }
};

var url = new URL(window.location.href);
Session.user  = Number(url.searchParams.get("user"));
Session.token = String(url.searchParams.get("token"));
console.log("user:",  Session.user);
console.log("token:", Session.token);

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

function new_path(msg)
{
	var path = msg["path"];
	path.date = [];
	path.loc = [];
	Session.paths[path.id] = path;
	
	// var v = {}
	// v.layerGroup = L.layerGroup();
	// v.circleMarker = [];

	var points = [[0,0]];
	for (let i = 0; i < path.loc.length; i++)
	{
		points.push(path.loc[i]);
		// v.circleMarker.push(L.circleMarker(points[i], {radius: (i==v.path.length-1?6:v.path[i].date.hour?1:3), color: icolor.value, fillOpacity: 0.6, stroke: false}).addTo(map));
		// v.layerGroup.addLayer(v.circleMarker[i]);
	}

	var polyline = L.polyline(points, {color: 'red', opacity: 0.2});
	//console.log(polyline);
	polyline.bindPopup("Hi");
	// var layerGroup = L.layerGroup();
	// layerGroup.addLayer(polyline);
	// layerGroup.addTo(map);
	polyline.addTo(map);

	Session.paths[path.id].polyline = polyline;
}

function update_path(msg)
{
	console.log(msg);
	var id = msg["id"];
	var step = msg["step"];

	for (let i = 0; i < msg.loc.length; i++)
	{
		Session.paths[id].loc[step + i]  = msg.loc[i];
		Session.paths[id].date[step + i] = msg.date[i];
		//points.push(path.loc[i]);
		// v.circleMarker.push(L.circleMarker(points[i], {radius: (i==v.path.length-1?6:v.path[i].date.hour?1:3), color: icolor.value, fillOpacity: 0.6, stroke: false}).addTo(map));
		// v.layerGroup.addLayer(v.circleMarker[i]);
	}
	Session.paths[id].polyline.setLatLngs(Session.paths[id].loc);
}
