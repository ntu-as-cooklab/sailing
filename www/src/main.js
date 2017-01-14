"use strict";

var container, controls, info, outputlist, doConnect, icolor, iyear, heading, project, dataset_avg, dataset_1979, dataset_1982;
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
	project 	= document.getElementById("project");

	dataset_avg 	= document.getElementById("dataset_avg");
	dataset_1979 	= document.getElementById("dataset_1979");
	dataset_1982 	= document.getElementById("dataset_1982");

	//alert(icolor.value);

	wsClient = new WsClient();
	wsClient.connect();

	initMap();
}

function setParam(e)
{
	if(e.name) wsClient.send(e.name + "= " + e.value);

	if(e == heading)
	{
		if(heading.value<0) heading.value = Number(heading.value) + 360;
		heading.value = heading.value%360;
		wsClient.send("dir= " + Math.sin(heading.value*Math.PI/180.0) + " " + Math.cos(heading.value*Math.PI/180.0));
	}

	var year;
	/*if (dataset_avg.checked)
	{
		wsClient.send("dataset= 2"); year = 9999;
	}
	if (dataset_1979.checked)
	{
		wsClient.send("dataset= 2"); year = 1979;
	}
	if (dataset_1982.checked)
	{
		wsClient.send("dataset= 2"); year = 1982;
	}*/

	if(e == iyear)
	{
		year = e.value;
		startdate.min = year + "-01-01";
		startdate.value = year + "-01-01";
		startdate.max = (Number(year)+1) + "-12-31";
		setDays();
		enddate.min = startdate.value;
		enddate.max = (Number(year)+1) + "-12-31";
		wsClient.send(startdate.name + "= " + startdate.value);
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

function newRecord()
{
	var projectName = prompt("請輸入專案名稱", "New");

    if (projectName != null)
	{
        project.innerHTML = projectName;
		wsClient.send("project= " + projectName);
    }

	for (var n=0; n<voyage.length; n++)
	{
		map.removeLayer(voyage[n].layerGroup);
		voyage[n].deleted = true;
		var item = document.getElementById("outputitem"+n);
		if(item) item.parentNode.removeChild(item);
	}
}

function setcolor(e, n)
{
	voyage[n].polyline.setStyle({color: e.value});
	voyage[n].polyline.redraw();
	for (var i = 0; i < voyage[n].path.length; i++)
	{
		voyage[n].circleMarker[i].setStyle({color: e.value});
		voyage[n].circleMarker[i].redraw();
	}
}

function setchecked(e, n)
{
	if (e.checked) voyage[n].layerGroup.addTo(map);
	else map.removeLayer(voyage[n].layerGroup);
}

function del(n)
{
	map.removeLayer(voyage[n].layerGroup);
	voyage[n].deleted = true;
	var item = document.getElementById("outputitem"+n);
	if(item) item.parentNode.removeChild(item);
	wsClient.send("delete " + voyage[n].name);
}

function clearRecord()
{
	for (var n=0; n<voyage.length; n++)
		del(n);
}

function toggleMode(i)
{
	if (i == 0)
	{
		document.getElementById("ship").disabled = true;
		document.getElementById("altitude").disabled = true;
		document.getElementById("windlimit").disabled = true;
		document.getElementById("sailopenhours").disabled = true;
	}
	else
	{
		document.getElementById("ship").disabled = false;
		document.getElementById("altitude").disabled = false;
		document.getElementById("windlimit").disabled = false;
		document.getElementById("sailopenhours").disabled = false;
	}
}
