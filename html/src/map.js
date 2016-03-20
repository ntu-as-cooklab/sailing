"use strict";

var map;
var loraLayer;
var balloon;
var infowindow;

function initMap() {
	map = new google.maps.Map(document.getElementById('map'), {
		center: {lat: 25.014852, lng: 121.538715},
		zoom: 8,
		mapTypeId: google.maps.MapTypeId.HYBRID,
		disableDefaultUI: false,
		zoomControl: false,
  		mapTypeControl: false,
  		scaleControl: false,
  		streetViewControl: false,
  		rotateControl: true,
  		fullscreenControl: false
	});

	// To show the latlng under mouse cursor.
	var coordsDiv = document.getElementById('coords');
	map.controls[google.maps.ControlPosition.TOP_CENTER].push(coordsDiv);
	map.addListener('mousemove', function(event) {
		coordsDiv.textContent = event.latLng.lat().toFixed(5) + ', ' + event.latLng.lng().toFixed(5);
	});

	// Executed after map loaded
	google.maps.event.addListenerOnce(map, 'idle', function(){mapLoaded();});

	var heatMapData = [
	  {location: new google.maps.LatLng(37.782, -122.447), weight: 0.5},
	  new google.maps.LatLng(37.782, -122.445),
	  {location: new google.maps.LatLng(37.782, -122.443), weight: 2},
	  {location: new google.maps.LatLng(37.782, -122.441), weight: 3},
	  {location: new google.maps.LatLng(37.782, -122.439), weight: 2},
	  new google.maps.LatLng(37.782, -122.437),
	  {location: new google.maps.LatLng(37.782, -122.435), weight: 0.5},

	  {location: new google.maps.LatLng(37.785, -122.447), weight: 3},
	  {location: new google.maps.LatLng(37.785, -122.445), weight: 2},
	  new google.maps.LatLng(37.785, -122.443),
	  {location: new google.maps.LatLng(37.785, -122.441), weight: 0.5},
	  new google.maps.LatLng(37.785, -122.439),
	  {location: new google.maps.LatLng(37.785, -122.437), weight: 2},
	  {location: new google.maps.LatLng(37.785, -122.435), weight: 3}
	];
	var heatmap = new google.maps.visualization.HeatmapLayer({
  		data: heatMapData
	});
	heatmap.setMap(map);


	// Add balloon marker
	//balloon = new google.maps.Marker({
	//	position: {lat: lora.data[1].latitude, lng: lora.data[1].longitude},
	//	map: map
	//});
	// Add info window
	//infowindow = new google.maps.InfoWindow({
  	//	content: lora.data[1].latitude + ", " + lora.data[1].longitude
	//});
	//balloon.addListener('click', function() {infowindow.open(map, balloon);});

	// Add lora layer
	//loraLayer = new google.maps.KmlLayer({
	//	url: 'http://mospc.cook.as.ntu.edu.tw/phormoza/data/0124.kml',
	//	map: map
	//});

	//map.overlayMapTypes.insertAt(0, map.mapTypes.coordinate);
}

function mapLoaded() {}
