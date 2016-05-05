"use strict";

var container, controls, info, outputlist, doConnect, icolor, iyear, heading;
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
	icolor 	= document.getElementById("icolor");
	iyear 	= document.getElementById("iyear");
	heading 	= document.getElementById("heading");

	//alert(icolor.value);

	wsClient = new WsClient();
	wsClient.connect();

	initMap();
}

function setParam(e)
{
	if(e.name) wsClient.send(e.name + "= " + e.value);
	if(e == iyear)
	{
		startdate.min = e.value + "-01-01";
		startdate.value = e.value + "-01-01";
		startdate.max = (e.value+1) + "-12-31";
		setDays();
		wsClient.send(startdate.name + "= " + startdate.value);
	}
	if(e == heading)
	{
		if(heading.value<0) heading.value = Number(heading.value) + 360;
		heading.value = heading.value%360;
		wsClient.send("dir= " + Math.sin(heading.value*Math.PI/180.0) + " " + Math.cos(heading.value*Math.PI/180.0));
	}
}
function setStartDate() { setEndDate(); enddate.min = startdate.value; if (new Date(enddate.value).getTime() < new Date(startdate.value).getTime()) enddate.value = startdate.value; setParam(enddate); }
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
