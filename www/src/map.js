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

	function onMapClick(e) { setOrig(curr_date, e.latlng); controls_updateui(); }
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

function addnewPath(path)
{
	//console.log(path);
	// var v = {}
	// v.layerGroup = L.layerGroup();
	// v.circleMarker = [];

	var points = [];
	for (var i = 0; i < path.length; i++)
	{
		points.push(path[i].loc);
		//console.log(path[i].loc);
		// v.circleMarker.push(L.circleMarker(points[i], {radius: (i==v.path.length-1?6:v.path[i].date.hour?1:3), color: icolor.value, fillOpacity: 0.6, stroke: false}).addTo(map));
		// v.layerGroup.addLayer(v.circleMarker[i]);
	}

	var polyline = L.polyline(points, {color: 'red', opacity: 0.2});
	polyline.bindPopup("Hi");
	// var layerGroup = L.layerGroup();
	// layerGroup.addLayer(polyline);
	// layerGroup.addTo(map);
	polyline.addTo(map);
}