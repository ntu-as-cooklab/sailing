"use strict";

// class WsClient
var WsClient = function()
{
	var ws;
	var url = "ws://127.0.0.1:8000";
	var connected = false;

	this.connect = function ()
	{
		if ("WebSocket" in window)
			ws = new WebSocket(url, ["lws-minimal"]);
		else if ("MozWebSocket" in window)
			ws = new MozWebSocket(url, ["lws-minimal"]);

		ws.binaryType = 'arraybuffer';

		ws.onopen = function(e)
		{
			console.log("connected");
			connected = true;
			//this.send("loginID= ");
			//this.send("runId= ");
		};
		ws.onerror = function(e) { console.log(e); };
		ws.onclose = function(e) { connected = false; console.log(e.code+(e.reason != "" ? ","+e.reason : ""));};

		ws.onmessage = function(e)
		{
			if (typeof e.data === "string")
				console.log(e.data);
			else {
				var msg = CBOR.decode(e.data);
				//console.log(msg);
				//console.log(msg["cmd"]);
				switch (msg["cmd"])
				{
					case "newPath":
						//console.log("addnewPath");
						addnewPath(msg["path"]);
					break;
					default:
						console.log("cmd", msg["cmd"]);
				}
			}
				
			// 		case "kml":
			// 			omnivore.kml(cmd[1].trim()).addTo(map);
			// 			break;
			// 		case "json":
			// 			var v = JSON.parse(e.data.slice(e.data.indexOf(cmd[0]) + cmd[0].length));

			// 			// Check if new voyage
			// 			var newv = true;
			// 			for (var i=0; i<voyage.length; i++) if (voyage[i].name == v.name && !voyage[i].deleted) newv = false;
			// 			if (newv)
			// 			{
			// 				voyage.push(v);
			// 				parseVoyage(voyage[voyage.length-1]);
			// 			} //else alert("此參數設定已計算過!");
			// 			break;
		};
	};

	this.disconnect = function() { ws.close(); };
	this.send = function(msg) { ws.send(msg); };
}

function addnewPath(path)
{
	//console.log(path);
	// var v = {}
	// v.layerGroup = L.layerGroup();
	// v.circleMarker = [];

	var points = [];
	for (var i = 0; i < path.length; i++)
	{
		points.push(path[i].loc);
		//console.log(path[i].loc);
		// v.circleMarker.push(L.circleMarker(points[i], {radius: (i==v.path.length-1?6:v.path[i].date.hour?1:3), color: icolor.value, fillOpacity: 0.6, stroke: false}).addTo(map));
		// v.layerGroup.addLayer(v.circleMarker[i]);
	}

	var polyline = L.polyline(points, {color: 'red', opacity: 0.2});
	// var layerGroup = L.layerGroup();
	// layerGroup.addLayer(polyline);
	// layerGroup.addTo(map);
	polyline.addTo(map);
}

function parseVoyage(v)
{
	v.layerGroup = L.layerGroup()
	v.circleMarker = [];
	var points = [];
	for (var i = 0; i < v.path.length; i++)
	{
		points.push(v.path[i].curr);
		v.circleMarker.push(L.circleMarker(points[i], {radius: (i==v.path.length-1?6:v.path[i].date.hour?1:3), color: icolor.value, fillOpacity: 0.6, stroke: false}).addTo(map));
		v.layerGroup.addLayer(v.circleMarker[i]);
	}
	v.polyline = L.polyline(points, {color: icolor.value, opacity: 0.2})
	v.layerGroup.addLayer(v.polyline);
	v.layerGroup.addTo(map);

	outputlist.innerHTML =
	"<div class='outputitem' id='outputitem"+(voyage.length-1)+"'>" +
	"<input type='checkbox' onchange='setchecked(this,"+(voyage.length-1)+")' checked> 顯示 " +
	"<input type='color' value="+icolor.value+" onchange='setcolor(this,"+(voyage.length-1)+")'> "+
	"<span style='position:absolute;right:10%'><input type='button' value='刪除' onclick='del("+(voyage.length-1)+")'></span></br>" +
	"起點: " + v.orig.lat + ", " + v.orig.lng + "</br>" +
	"資料: " + datasetName[v.dataset] + "</br>" +
	"時間: " +
	v.startdate.year + "/" + (v.startdate.month<10?"0":"") + v.startdate.month + "/" + (v.startdate.day<10?"0":"") + v.startdate.day + " " + v.startdate.hour + "hr" +
	" - " +
	v.enddate.year + "/" + (v.enddate.month<10?"0":"") + v.enddate.month + "/" + (v.enddate.day<10?"0":"") + v.enddate.day + " " + v.enddate.hour + "hr" +
	"</br>" +
	"天數: " + v.days + "</br>" +
	"模式: " + modeName[v.mode] + "</br>" +
	"風帆高度: " + v.altitude + "m</br>" +
	"張帆極限風速: " + v.windlimit + "m/s</br>" +
	"每日張帆時數: " + v.sailopenhours + "hr</br>" +
	"檔案名稱: " + v.name +
	"</br><a href='output/" + v.name + ".csv' download class='button'>下載</a>" +
	"</div>" +
	outputlist.innerHTML;
}
