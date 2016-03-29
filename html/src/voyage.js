"use strict";

var LatLon = function()
{
	this.lat = 0; this.lon = 0;
}

var UV = function()
{
	this.u = 0; this.v = 0;
}

var Voyage = function()
{
	this.orig = new LatLon();
	this.dest = new LatLon();
	this.dir = new UV();
	this.timestep = 3600;
	this.movement_factor = 1;
	this.alpha = 0.11;
	this.altitude = 2;
	this.range = 1.1;
	this.sail_open = false;
	this.run = function() { send("run"); };
};
