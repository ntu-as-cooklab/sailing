"use strict";

var container, controls, info;
var wsClient;

function main()
{
	// Get DOM elements
	container 	= document.getElementById("container");
	controls 	= document.getElementById("controls");
	info 		= document.getElementById("info");

	wsClient = new WsClient();
	wsClient.connect();

	initMap();
}

function setParam(e) { if(e.name) wsClient.send(e.name + "= " + e.value); }
