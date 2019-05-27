"use strict";

function pad(n, width, z)
{
	z = z || '0';
	n = n + '';
	return n.length >= width ? n : new Array(width - n.length + 1).join(z) + n;
}

function date2str(date)
{
	return `${date.getFullYear()}-${(date.getMonth()+1).toString().padStart(2,'0')}-${date.getDate().toString().padStart(2,'0')}`;
}

function time2str(date)
{
	return `${date.getHours().toString().padStart(2,'0')}:${date.getMinutes().toString().padStart(2,'0')}`;
}

function loc2str(loc)
{
	return `${loc[0].toFixed(6)}, ${loc[1].toFixed(6)}`;
}

function latlng2str(latlng)
{
	return `${latlng.lat.toFixed(6)}, ${latlng.lng.toFixed(6)}`;
}

function date2array(date)
{
	return [date.getYear(),date.getMonth(),date.getDay(),date.getHours()];
}

function loc2array(loc)
{
	return [loc.lat, loc.lng];
}