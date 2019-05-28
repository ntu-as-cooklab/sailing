"use strict";

function date2str(date)
{
	return `${date.getFullYear()}-${(date.getMonth()+1).toString().padStart(2,'0')}-${date.getDate().toString().padStart(2,'0')}`;
}

function time2str(date)
{
	return `${date.getHours().toString().padStart(2,'0')}:${date.getMinutes().toString().padStart(2,'0')}`;
}

function wraplng(lng)
{
	var lng_ = Math.fmod(lng, 360);
	if (lng_ >= 180) lng_ -= 360;
	return lng_;
}

function loc2str(loc)
{
	return `${loc[0].toFixed(6)}, ${wraplng(loc[1]).toFixed(6)}`;
}

function latlng2str(latlng)
{
	return `${latlng.lat.toFixed(6)}, ${wraplng(latlng.lng).toFixed(6)}`;
}

function date2array(date)
{
	return [date.getYear(),date.getMonth(),date.getDate(),date.getHours()];
}

function loc2array(loc)
{
	return [loc.lat, loc.lng];
}