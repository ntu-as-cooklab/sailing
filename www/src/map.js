"use strict";

var map;
var startloc_marker;

var mainlatlonlines;
var latlonlines;

function startloc_input(e)
{
	Session.startloc = [e.latlng.lat, e.latlng.lng];
	startloc_marker.setLatLng(Session.startloc);
	$("#startloc")[0].innerText = loc2str(Session.startloc);
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
            // {text: '新增路徑', callback: function(e){newPath(curr_date, e.latlng)} },
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
	startloc_marker = L.marker([3.0, 160.0], { icon: L.icon({
			iconUrl: 	'res/Celtic-style_crossed_circle.svg.png',
			iconSize:     [iconsize, iconsize], // size of the icon
			iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
			popupAnchor:  [0, -iconsize/2] // point from which the popup should open relative to the iconAnchor
			}),
		draggable: false, continousWorld : true }).addTo(map);
	startloc_marker.setLatLng(Session.startloc);
	map.on('click', startloc_input);
	//startloc_marker.on('dragend', function(e){startloc_input(e.target);});

	// L.easyButton('fa-globe', function(btn, map){
	// 	var antarctica = [-77,70];
	// 	map.setView(antarctica);
	// }, "New").addTo(map);

	/** Events **/
	//map.on('contextmenu', onMapClick);
	map.on('mousemove', function(e){ $("#info")[0].innerText = latlng2str(e.latlng); });
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