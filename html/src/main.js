"use strict";

var infobox, timeline;

function main()
{
	// Get DOM elements
	infobox 	= document.getElementById("infobox");
	timeline 	= document.getElementById("timeline");

	initMap();
}

function initUI() {
	//timeline.min = Time.valueOf(lora.data[1].time);
	//timeline.max = Time.valueOf(lora.data[lora.data.length-1].time);
	//timeline.defaultValue = timeline.min;
	//timeline.value = timeline.min;
	//timestart.innerHTML = Time.toString(timeline.min);
	//timeend.innerHTML 	= Time.toString(timeline.max);
}

function update() {

	// Update timeline
	//timecurrent.innerHTML = Time.toString(timeline.value);
	//timecurrent.style.left = 1 + (timeline.value - timeline.min)*92/(timeline.max - timeline.min) + "%";

	// Update infobox
	//var show = ["time", "package_no", "altitude", "temperature", "pressure", "RSSI", "frequency_error"];
	//var info = "";
	//for (var i=0; i<show.length; i++)
	//	info += "<tr>"
	//		+ "<td>" + lora.display_name[show[i]] + ": " + "</td>"
	//		+ "<td>" + lora.currentData()[show[i]] + " " + lora.units[show[i]] + "</td>"
	//		+ "</tr>";
	//infobox.innerHTML = info;

	// Update map
	//balloon.setPosition(new google.maps.LatLng(lora.currentData().latitude, lora.currentData().longitude));
	//infowindow.setContent(lora.currentData().latitude + ", " + lora.currentData().longitude);
};

function play() {}
function speedUp() {}
function speedDown() {}
