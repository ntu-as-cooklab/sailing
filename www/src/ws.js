"use strict";

var ws;

function ws_init()
{
	var url = "ws://" + window.location.host;
	ws = new ReconnectingWebSocket(url, ["lws-minimal"], {debug: false, reconnectInterval: 1000});

	ws.binaryType = 'arraybuffer';

	ws.onopen = function(e) {
		console.log("Websocket connected");
		$("#offline").hide();
		session_restore();
	};

	ws.onerror = function(e) {
		console.error("Websocket error: ", e); 
	};

	ws.onclose = function(e) {
		console.log("Websocket closed: ", e.code, e.reason);
		$("#offline").show();
	};

	ws.onmessage = function(e) {
		if (typeof e.data === "string")
			console.log(e.data);
		else {
			var msg = CBOR.decode(e.data);
			//console.log(msg["cmd"]);
			switch (msg["cmd"])
			{
				case "new_path":
					path_new(msg);
					break;
				case "pts":
					path_update(msg);
					break;
				default:
					console.log("Unrecognized command: ", msg["cmd"]);
			}
		}
	};
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