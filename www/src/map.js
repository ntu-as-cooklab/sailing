"use strict";

var map;
var orig, dest;
var datasetName = ["1979~2009 平均資料", "1979~2009 連續資料", "單年資料"];
var modeName = ["瓶中信 (無風帆)", "隨風漂流 (航向=風向)", "固定目的地", ""];

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
		<div>${latlng2str(latlng)}</div>
		<div>
			<input type="date" min="${date2str(min_date)}" max="${date2str(max_date)}" value="${date2str(date)}"></input>
			<input type="time" value="${time2str(date)}" step="3600"></input>
		</div>
		<div><button>確定</button></div>
	`).openPopup();
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
			popupAnchor:  [0, 0] // point from which the popup should open relative to the iconAnchor
			}),
		draggable: true, continousWorld : true }).addTo(map);

	// var destIcon = L.icon({
	// 		iconUrl: 	'dest.png',
	// 		iconSize:     [iconsize, iconsize], // size of the icon
	// 		iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
	// 		popupAnchor:  [0, iconsize/2] // point from which the popup should open relative to the iconAnchor
	// });

	//function setDest(latlng) { dest ? dest.setLatLng(latlng) : dest = L.marker(latlng, { icon: destIcon, draggable: true, continousWorld : true }).addTo(map); }
	setOrig(curr_date, new L.LatLng(-10.0, 160.0));
	//setDest(new L.LatLng(-14.0, 188.5));

	L.easyButton('fa-globe', function(btn, map){
		var antarctica = [-77,70];
		map.setView(antarctica);
	}, "New").addTo(map);

	/** Events **/

	function onMapClick(e) { setOrig(curr_date, e.latlng); }
	function onMapMouseMove(e) { info.innerHTML = e.latlng.lat.toFixed(5) + ', ' + e.latlng.lng.toFixed(5); }
	function onOrigDragEnd(e) { wsClient.send("orig = " + orig.getLatLng().lat + "," + orig.getLatLng().lng); }
	function onDestDragEnd(e) { wsClient.send("dest = " + dest.getLatLng().lat + "," + dest.getLatLng().lng); }
	map.on('click', onMapClick);
	map.on('contextmenu', onMapClick);
	map.on('mousemove', onMapMouseMove);
	orig.on('dragend', onOrigDragEnd);
	dest.on('dragend', onDestDragEnd);

	toggleLatLonLines();
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
