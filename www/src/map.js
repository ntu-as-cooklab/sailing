"use strict";

var map;
var orig, dest;

var mainlatlonlines;
var latlonlines;

var min_date = new Date(1979, 0,  1, 0);
var max_date = new Date(2011, 11, 31, 0);
var curr_date = min_date;

function newPath(date, orig)
{
	console.log("newPath");
	wsClient.send(CBOR.encode({
		cmd: "newPath",
		date: [date.getYear(),date.getMonth(),date.getDay(),date.getHours()],
		orig: [orig.lat, orig.lng],
	}));
}

function setOrig(date, latlng)
{
	orig.setLatLng(latlng); 
	orig.bindPopup(`
		<table>
		<tr><td>起始地點: </td><td>${latlng2str(latlng)}</td></tr>
		<tr>
			<td>起始時間: </td>
			<td>
				<input type="date" min="${date2str(min_date)}" max="${date2str(max_date)}" value="${date2str(date)}"></input>
				<input type="time" value="${time2str(date)}" step="3600"></input>
			</td>
		</tr>
		<tr>
			<td>結束時間: </td>
			<td>
				<input type="date" min="${date2str(min_date)}" max="${date2str(max_date)}" value="${date2str(date)}"></input>
				<input type="time" value="${time2str(date)}" step="3600"></input>
			</td>
		</tr>
		<tr>
			<td>時間長度: </td>
			<td>
				<input style="width:3.5em" type="number" min="1" value="1">天
				<input style="width:2.5em" type="number" min="1" value="0">小時</input>
			</td>
		</tr>
		<tr><td><button onclick="generatePath()">產生路徑</button></td></tr>
		<table>
	`).openPopup();
}

function generatePath()
{
	newPath(curr_date, orig.getLatLng());
}

function initMap()
{
	map = L.map('map', {
		//worldCopyJump: true,
		minZoom: 3,
		maxZoom: 18,
		'center': [3.0, 160.0],
		'zoom': 4,
		inertia : false,
		//fadeAnimation: false,
		//zoomAnimation: false,
		contextmenu: true,
        contextmenuWidth: 140,
        contextmenuItems: [
            {text: '新增路徑', callback: function(e){newPath(curr_date, e.latlng)} },
		]
	});

	L.tileLayer('https://api.tiles.mapbox.com/v4/{id}/{z}/{x}/{y}.png?access_token={accessToken}', {
			attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="http://mapbox.com">Mapbox</a>, <p>Flat Icons by <a href="/resources/free-icons/">WhoIsHostingThis.com</a></p>',
			id: 'seanstone5923.phlaakpk',
			accessToken: 'pk.eyJ1Ijoic2VhbnN0b25lNTkyMyIsImEiOiJjaW1kOTJrZGYwMDUxdHNra2wwOGN0cTY1In0.n0m5Euk9SWbiNt4RZi8vkQ',
	}).addTo(map);

	L.control.scale().addTo(map);

	/** Markers **/

	var iconsize = 32;
	orig = L.marker([3.0, 160.0], { icon: L.icon({
			iconUrl: 	'Celtic-style_crossed_circle.svg.png',
			iconSize:     [iconsize, iconsize], // size of the icon
			iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
			popupAnchor:  [0, -iconsize/2] // point from which the popup should open relative to the iconAnchor
			}),
		draggable: true, continousWorld : true }).addTo(map);

	setOrig(curr_date, new L.LatLng(-10.0, 160.0));

	L.easyButton('fa-globe', function(btn, map){
		var antarctica = [-77,70];
		map.setView(antarctica);
	}, "New").addTo(map);

	/** Events **/

	function onMapClick(e) { setOrig(curr_date, e.latlng); }
	function onMapMouseMove(e) { document.getElementById("info").innerHTML = e.latlng.lat.toFixed(5) + ', ' + e.latlng.lng.toFixed(5); }
	//function onOrigDragEnd(e) { wsClient.send("orig = " + orig.getLatLng().lat + "," + orig.getLatLng().lng); }
	map.on('click', onMapClick);
	map.on('contextmenu', onMapClick);
	map.on('mousemove', onMapMouseMove);
	//orig.on('dragend', onOrigDragEnd);

	//toggleLatLonLines();
}

function toggleLatLonLines()
{
	var showLatLonLines = document.getElementById("showLatLonLines");
	if(showLatLonLines.checked)
	{
		// Specify divisions every 10 degrees
		latlonlines = L.graticule({ interval: 10 }).addTo(map);

		// Specify bold red lines instead of thin grey lines
		mainlatlonlines = L.graticule({
		style: {
	        	color: '#f00',
	        	weight: 1
	    	}
		}).addTo(map);
	}
   	else
	{
        map.removeLayer(latlonlines);
		map.removeLayer(mainlatlonlines);
	}
}
