"use strict";

var UV = function()
{
	this.u = 0; this.v = 0;
}

var Voyage = function()
{
	this.orig = new L.LatLng(26.0, 133.0);
	this.dest = new L.LatLng(27.0, 134.0);
	this.dir = new UV();
	this.timestep = 3600;
	this.movement_factor = 1;
	this.alpha = 0.11;
	this.altitude = 2;
	this.range = 1.1;
	this.sail_open = false;
	this.Run = function() { send("run"); };
};

var voyage = new Voyage();
