"use strict";

var map;
var startloc_marker, destloc_marker;
var mainlatlonlines, latlonlines;

function startloc_input(e)
{
	Session.startloc = [e.latlng.lat, e.latlng.lng];
	startloc_marker.setLatLng(Session.startloc);
	$("#startloc")[0].innerText = loc2str(Session.startloc);
}

function startloc_init()
{
	var iconsize = 32;
	startloc_marker = L.marker(Session.startloc, { icon: L.icon({
			iconUrl: 	'res/orig.png',
			iconSize:     [iconsize, iconsize], // size of the icon
			iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
			popupAnchor:  [0, -iconsize/2] // point from which the popup should open relative to the iconAnchor
			}),
		draggable: false, continousWorld : true }).addTo(map);
	startloc_marker.setLatLng(Session.startloc);
	map.on('click', startloc_input);
}

function destloc_input(e)
{
	Session.destloc = [e.latlng.lat, e.latlng.lng];
	destloc_marker.setLatLng(Session.destloc);
	$("#destloc")[0].innerText = loc2str(Session.destloc);
}

function destloc_show()
{
	var iconsize = 32;
	destloc_marker = L.marker(Session.destloc, { icon: L.icon({
			iconUrl: 	'res/dest.png',
			iconSize:     [iconsize, iconsize], // size of the icon
			iconAnchor:   [iconsize/2, iconsize/2], // point of the icon which will correspond to marker's location
			popupAnchor:  [0, -iconsize/2] // point from which the popup should open relative to the iconAnchor
			}),
		draggable: false, continousWorld : true }).addTo(map);
		destloc_marker.setLatLng(Session.destloc);
	map.on('contextmenu', destloc_input); // rightclick
}

function destloc_hide()
{
	if (map.hasLayer(destloc_marker))
		map.removeLayer(destloc_marker);
}

function initMap()
{
	map = L.map('map', {
		//worldCopyJump: true,
		//fadeAnimation: false,
		//zoomAnimation: false,
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

	L.tileLayer('https://api.mapbox.com/styles/v1/{id}/tiles/{z}/{x}/{y}?access_token={accessToken}', {
			attribution: `Map data &copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors, <a href="http://creativecommons.org/licenses/by-sa/2.0/">CC-BY-SA</a>, 
						Imagery © <a href="http://mapbox.com">Mapbox</a>
						<p>Flat Icons by <a href="/resources/free-icons/">WhoIsHostingThis.com</a></p>
						<p>©臺大大氣系—林博雄教授實驗室、臺師大科教所—許瑛玿教授實驗室、麗山高中—萬義昞老師</p>
						`,
			id: 'mapbox/streets-v11',
			accessToken: 'pk.eyJ1Ijoic2VhbnN0b25lNTkyMyIsImEiOiJjaW1kOTJrZGYwMDUxdHNra2wwOGN0cTY1In0.n0m5Euk9SWbiNt4RZi8vkQ',
	}).addTo(map);

	L.control.scale().addTo(map);

	/** Markers **/

	var myRepeatingMarkers = L.gridLayer.repeatedMarkers().addTo(map);

	startloc_init();

	grid_show();
}

function grid_show()
{
	latlonlines = L.latlngGraticule({
        showLabel: true,
        zoomInterval: [
            {start: 2, end: 3, interval: 30},
            {start: 4, end: 4, interval: 10},
            {start: 5, end: 7, interval: 5},
            {start: 8, end: 10, interval: 1}
		],
		color: "#666",
		fontColor: "#666"
    }).addTo(map);

	mainlatlonlines = L.latlngGraticule({	
		color: '#f00',
		weight: 2,
		zoomInterval: {
			latitude: [
			  {start: 1, end: 20, interval: 90},
			],
			longitude: [
			  {start: 1, end: 20, interval: 180},
			]
		  }
	}).addTo(map);
}

function grid_hide()
{
	map.removeLayer(latlonlines);
	map.removeLayer(mainlatlonlines);
}