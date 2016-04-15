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

		ws.onclose = function(e) {
			console.log(e.code+(e.reason != "" ? ","+e.reason : ""));
		};

		ws.onmessage = function(e) {
			console.log(e.data);
			omnivore.kml(e.data).addTo(map);
		};
	};

	this.disconnect = function() { ws.close(); };
	this.send = function(msg) { ws.send(msg); };
}
