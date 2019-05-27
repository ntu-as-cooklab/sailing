"use strict";

$(function(){main();});

function main()
{
	ws_init();
	initMap();
}

function setcolor(e, n)
{
	voyage[n].polyline.setStyle({color: e.value});
	voyage[n].polyline.redraw();
	for (var i = 0; i < voyage[n].path.length; i++)
	{
		voyage[n].circleMarker[i].setStyle({color: e.value});
		voyage[n].circleMarker[i].redraw();
	}
}

function setchecked(e, n)
{
	if (e.checked) voyage[n].layerGroup.addTo(map);
	else map.removeLayer(voyage[n].layerGroup);
}

function del(n)
{
	map.removeLayer(voyage[n].layerGroup);
	voyage[n].deleted = true;
	var item = document.getElementById("outputitem"+n);
	if(item) item.parentNode.removeChild(item);
	ws.send("delete " + voyage[n].name);
}