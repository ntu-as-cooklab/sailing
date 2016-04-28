"use strict";

var container, controls, info, outputlist, doConnect;
var startdate, enddate, days;
var wsClient;
var voyage = [];

function main()
{
	// Get DOM elements
	container 	= document.getElementById("container");
	controls 	= document.getElementById("controls");
	info 		= document.getElementById("info");
	startdate 	= document.getElementById("startdate");
	enddate 	= document.getElementById("enddate");
	days 		= document.getElementById("days");
	outputlist 	= document.getElementById("outputlist");
	doConnect 	= document.getElementById("doConnect");

	wsClient = new WsClient();
	wsClient.connect();

	initMap();
}

function setParam(e) { if(e.name) wsClient.send(e.name + "= " + e.value); }
function setStartDate() { enddate.min = startdate.value; if (new Date(enddate.value).getTime() < new Date(startdate.value).getTime()) enddate.value = startdate.value; setParam(enddate); }
function setEndDate() { days.value = Math.ceil((new Date(enddate.value).getTime() - new Date(startdate.value).getTime())/86400000); setParam(days); }
function setDays() { var date = new Date(startdate.value);
						date.setTime(date.getTime() + days.value * 86400000);
						enddate.value = date.toISOString().substr(0,10); setParam(enddate); }

function connect()
{
	wsClient.connect();
}

function resetParams()
{
	
}
