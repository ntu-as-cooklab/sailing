"use strict";

var map;
var mode = "";
var pos;

function initMap() {

	var map = L.map('map', {
		'center': [26.0, 133.0],
		'zoom': 5
	});

	L.tileLayer('https://api.tiles.mapbox.com/v4/{id}/{z}/{x}/{y}.png?access_token={accessToken}', {
    	attribution: 'Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, Imagery © <a href="http://mapbox.com">Mapbox</a>',
    	maxZoom: 18,
    	id: 'seanstone5923.phlaakpk',
    	accessToken: 'pk.eyJ1Ijoic2VhbnN0b25lNTkyMyIsImEiOiJjaW1kOTJrZGYwMDUxdHNra2wwOGN0cTY1In0.n0m5Euk9SWbiNt4RZi8vkQ'
	}).addTo(map);

	L.control.scale().addTo(map);

	/** Events **/

	map.on('click', onMapClick);
	map.on('mousemove', onMapMouseMove);

	//** Toolbar **/

	var SetOrigAction = L.ToolbarAction.extend({
			options: { toolbarIcon: { html: '&#9873;', tooltip: "設定起點" } },
			addHooks: function () { mode = "設定起點"; showInfo(pos); }
		});
	var SetDestAction = L.ToolbarAction.extend({
			options: { toolbarIcon: { html: '&#9873;', tooltip: "設定目的地" } },
			addHooks: function () { mode = "設定目的地"; showInfo(pos); }
		});
	new L.Toolbar.Control({
		actions: [SetOrigAction, SetDestAction],
		position: 'topleft',
		className: 'leaflet-draw-toolbar'
	}).addTo(map);
}

function showInfo(pos) {
    info.innerHTML = "<h1>" + mode + "</h1>" + pos.lat.toFixed(5) + ', ' + pos.lng.toFixed(5);
}

function onMapClick(e) {
    //alert("You clicked the map at " + e.latlng);
}

function onMapMouseMove(e) {
	// To show the latlng under mouse cursor.
	pos = e.latlng;
	showInfo(pos);
}
