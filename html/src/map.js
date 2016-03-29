"use strict";

var map;

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

	map.on('click', onMapClick);
	map.on('mousemove', onMapMouseMove);

	var MyCustomAction = L.ToolbarAction.extend({

            options: {
                toolbarIcon: {
                    html: '&#9873;',
                    tooltip: 'Go to the Eiffel Tower'
                }
            },

            addHooks: function () {
                map.setView([48.85815, 2.29420], 19);
            }

        });

        new L.Toolbar.Control({
           actions: [MyCustomAction],
			position: 'topleft',
			className: 'leaflet-draw-toolbar'
        }).addTo(map);
}

function onMapClick(e) {
    //alert("You clicked the map at " + e.latlng);
}

function onMapMouseMove(e) {
	// To show the latlng under mouse cursor.
	coords.textContent = e.latlng.lat.toFixed(5) + ', ' + e.latlng.lng.toFixed(5);
}
