"use strict";

// class WsClient
var WsClient = function()
{
	var ws;
	var url = "ws://127.0.0.1";

	this.connect = function ()
	{
		if ("WebSocket" in window)
			ws = new WebSocket(url);
		else if ("MozWebSocket" in window)
			ws = new MozWebSocket(url);

		ws.onopen = function(e) {};
		ws.onerror = function(e) { console.log(e); };
		ws.onclose = function(e) { console.log(e.code+(e.reason != "" ? ","+e.reason : ""));};

		ws.onmessage = function(e)
		{
			console.log(e.data);
			var cmd = e.data.split('=');
			if (cmd.length == 2)
				switch (cmd[0].trim())
				{
					case "startdate":
						break;
					case "orig":
						orig.setLatLng(cmd[1].match(/\S+/g));
						break;
					case "dest":
						dest.setLatLng(cmd[1].match(/\S+/g));
						break;
				}
			else if (cmd.length == 1)
			{
				cmd = e.data.split(' ');
				switch (cmd[0].trim())
				{
					case "voyage":
						omnivore.kml(cmd[1].trim()).addTo(map);
						break;
				}
			}

		};
	};

	this.disconnect = function() { ws.close(); };
	this.send = function(msg) { ws.send(msg); };
}
