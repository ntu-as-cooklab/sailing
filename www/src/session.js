"use strict";

var Session = {
    user: 0,
    token: "",
    startdate:  DATE_MIN,
    enddate:    new Date(1979, 3, 1, 0),
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

function popup(path)
{
	return `<div class="popup">
		<table>
			<tr><td>路徑ID:</td><td>${path.id}</td></tr>
			<tr><td>建立者ID:</td><td>${path.user}</td></tr>
			<tr><td>起始時間:</td><td>${date2str(path.startdate) + " " + time2str(path.startdate)}</td></tr>
			<tr><td>結束時間:</td><td>${date2str(path.enddate) + " " + time2str(path.enddate)}</td></tr>
			<tr><td>時間長度:</td><td>${dates2intervalstr(path.startdate,path.enddate)}</td></tr>
			<tr><td>起始地點:</td><td>${loc2str(path.startloc)}</td></tr>
			<tr><td>結束地點:</td><td>${path.loc.length > 0 ? loc2str(path.loc[path.loc.length-1]) : ""}</td></tr>
		</table>
		<div>
			<button onclick="hide_path(${path.id})">隱藏</button>
			<button>下載csv</button>
			<button>下載kml</button>
			<button>刪除</button>
		</div>
	</div>`;
}

function new_path(msg)
{
	var path = msg["path"];
	Session.paths[path.id] = path;
	path.date = [];
	path.loc = [];
	path.startdate = array2date(path.startdate);
	path.enddate = array2date(path.enddate);

	var options = {color: 'red', opacity: 0.2, weight: 5};
	if (path.user != Session.user) options.color = 'grey';
	path.polyline = L.polyline([[]], options);
	path.polyline.addTo(map);

	path.circles = L.layerGroup();
}

function update_path(msg)
{
	var id = msg["id"];
	var step = msg["step"];
	var path = Session.paths[id];

	for (let i = 0; i < msg.loc.length; i++)
	{
		path.loc[step+i]  = msg.loc[i];
		path.date[step+i] = array2date(msg.date[i]);

		if (path.date[step+i].getHours() == 0) {
			var options = {radius: 1.2, color: 'red', fillOpacity: 0.9, stroke: false};
			var circle = L.circleMarker(msg.loc[i], options);
			circle.bindTooltip(`${date2str(path.date[step+i])} (${loc2str(path.loc[step+i])})`);
			path.circles.addLayer(circle);
		}
	}
	path.polyline.setLatLngs(path.loc);

	var options = {weight: 1, radius: 3, color: 'black', fillOpacity: 1, stroke: false};
	if (step == 0) path.circles.addLayer(L.circleMarker(msg.loc[0], options));
	path.circles.addTo(map);

	if (path.date[path.date.length-1] >= path.enddate)
		path.polyline.bindPopup(popup(path), {closeOnClick: false, autoClose: false}).openPopup();
}

function hide_path(id)
{
	var path = Session.paths[id];
	if(map.hasLayer(path.polyline)) map.removeLayer(path.polyline);
	if(map.hasLayer(path.circles)) map.removeLayer(path.circles);
}
