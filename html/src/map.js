"use strict";

var map;
var mode = "";
var cursor_pos = [26.0, 133.0];

var orig;
var dest;

function initMap() {

	map = L.map('map', {
		worldCopyJump: true,
		minZoom: 3,
		maxZoom: 18,
		'center': [26.0, 133.0],
		'zoom': 5,
		inertia : false
		//fadeAnimation: false,
		//zoomAnimation: false
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

	//** Toolbar **/

	var SetOrigAction = L.ToolbarAction.extend({
			options: { toolbarIcon: { html: '起點', tooltip: "設定起點" } },
			addHooks: function () { switchMode("設定起點");  }
		});
	var SetDestAction = L.ToolbarAction.extend({
			options: { toolbarIcon: { html: '目的', tooltip: "設定目的地" } },
			addHooks: function () { switchMode("設定目的地"); }
		});
	var SetModelAction = L.ToolbarAction.extend({
			options: { toolbarIcon: { html: '模式', tooltip: "設定模式" } },
			addHooks: function () { }
		});
	var SetDirAction = L.ToolbarAction.extend({
			options: { toolbarIcon: { html: '航向', tooltip: "設定航向" } },
			addHooks: function () { switchMode("設定航向"); }
		});
	var SailAction = L.ToolbarAction.extend({
			options: { toolbarIcon: { html: '計算', tooltip: "計算航道" } },
			addHooks: function () { send("run"); }
		});
	new L.Toolbar.Control({
		actions: [SetModelAction, SailAction],
		position: 'topleft',
		className: 'leaflet-draw-toolbar'
	}).addTo(map);

	/** Markers **/

	var iconsize = 32;

	var origIcon = L.icon({
	    	iconUrl: 	'html/orig.png',
	    	iconSize:     [iconsize, iconsize], // size of the icon
	    	iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
	    	popupAnchor:  [0, iconsize/2] // point from which the popup should open relative to the iconAnchor
	});

	var destIcon = L.icon({
			iconUrl: 	'html/dest.png',
			iconSize:     [iconsize, iconsize], // size of the icon
			iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
			popupAnchor:  [0, iconsize/2] // point from which the popup should open relative to the iconAnchor
	});

	orig = L.marker(voyage.orig, { icon: origIcon, draggable: true, continousWorld : true }).addTo(map);
	dest = L.marker(voyage.dest, { icon: destIcon, draggable: true, continousWorld : true  }).addTo(map);

	/** Events **/

	map.on('click', onMapClick);
	map.on('mousemove', onMapMouseMove);
	orig.on('dragend', onOrigDragEnd);
	dest.on('dragend', onDestDragEnd);
}

function showInfo(latlng) {
    info.innerHTML = "<h1 style='color:#ff4500'>" + mode + "</h1>" + latlng.lat.toFixed(5) + ', ' + latlng.lng.toFixed(5);
}

function onMapClick(e) {
	switch (mode) {
		case "設定起點":
			setOrig(e.latlng);
			break;
		case "設定目的地":
			setDest(e.latlng);
			break;
	}
	switchMode("");
}

function onMapMouseMove(e) {
	// To show the latlng under mouse cursor.
	cursor_pos = e.latlng;
	showInfo(cursor_pos);
}

function onOrigDragEnd(e) {
	setOrig(orig.getLatLng());
}

function onDestDragEnd(e) {
	setDest(dest.getLatLng());
}

function switchMode(m) {
	mode = m;
	showInfo(cursor_pos);
}

function setOrig(latlng) {
	orig.setLatLng(voyage.orig = latlng);
	send("orig= " + latlng.lat + " " + latlng.lng);
}

function setDest(latlng) {
	dest.setLatLng(voyage.dest = latlng);
	send("dest= " + latlng.lat + " " + latlng.lng);
}
