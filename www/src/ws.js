"use strict";

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