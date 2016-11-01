"use strict";

var map;
var orig, dest;
var datasetName = ["1979~2009 平均資料", "1979~2009 連續資料", "單年資料"];
var modeName = ["瓶中信 (無風帆)", "隨風漂流 (航向=風向)", "固定目的地", ""];

function initMap()
{
	map = L.map('map', {
		worldCopyJump: true,
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
			{text: '設為起點', callback: function(e){setOrig(e.latlng);onOrigDragEnd(e);} },
			{text: '設為目的地', callback: function(e){setDest(e.latlng);onDestDragEnd(e)} },
		]
	});

	L.tileLayer('https://api.tiles.mapbox.com/v4/{id}/{z}/{x}/{y}.png?access_token={accessToken}', {
    	attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="http://mapbox.com">Mapbox</a>, <p>Flat Icons by <a href="/resources/free-icons/">WhoIsHostingThis.com</a></p>',
    	id: 'seanstone5923.phlaakpk',
    	accessToken: 'pk.eyJ1Ijoic2VhbnN0b25lNTkyMyIsImEiOiJjaW1kOTJrZGYwMDUxdHNra2wwOGN0cTY1In0.n0m5Euk9SWbiNt4RZi8vkQ',
		//unloadInvisibleTiles : false,
		//reuseTiles : true,
		//updateWhenIdle : false,
		continousWorld : true,
		//noWrap: false
	}).addTo(map);

	L.control.scale().addTo(map);

	/** Markers **/

	var iconsize = 32;
	var origIcon = L.icon({
			iconUrl: 	'orig.png',
			iconSize:     [iconsize, iconsize], // size of the icon
			iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
			popupAnchor:  [0, iconsize/2] // point from which the popup should open relative to the iconAnchor
	});
	var destIcon = L.icon({
			iconUrl: 	'dest.png',
			iconSize:     [iconsize, iconsize], // size of the icon
			iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
			popupAnchor:  [0, iconsize/2] // point from which the popup should open relative to the iconAnchor
	});
	function setOrig(latlng) { orig ? orig.setLatLng(latlng) : orig = L.marker(latlng, { icon: origIcon, draggable: true, continousWorld : true }).addTo(map); }
	function setDest(latlng) { dest ? dest.setLatLng(latlng) : dest = L.marker(latlng, { icon: destIcon, draggable: true, continousWorld : true }).addTo(map); }
	setOrig(new L.LatLng(-10.0, 160.0));
	setDest(new L.LatLng(-14.0, 188.5));

	/** Events **/

	function onMapClick(e) {}
	function onMapMouseMove(e) { info.innerHTML = e.latlng.lat.toFixed(5) + ', ' + e.latlng.lng.toFixed(5); }
	function onOrigDragEnd(e) { wsClient.send("orig = " + orig.getLatLng().lat + "," + orig.getLatLng().lng); }
	function onDestDragEnd(e) { wsClient.send("dest = " + dest.getLatLng().lat + "," + dest.getLatLng().lng); }
	map.on('click', onMapClick);
	map.on('mousemove', onMapMouseMove);
	orig.on('dragend', onOrigDragEnd);
	dest.on('dragend', onDestDragEnd);

	// Add a basic graticule with divisions every 20 degrees
	// as a layer on a map
	L.graticule().addTo(map);

	// Specify divisions every 10 degrees
	//L.graticule({ interval: 180 }).addTo(map);

	// Specify bold red lines instead of thin grey lines
	L.graticule({
	style: {
        	color: '#f00',
        	weight: 1
    	}
	}).addTo(map);
}
