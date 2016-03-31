var container, controls, coords;

function main()
{
	// Get DOM elements
	container 	= document.getElementById("container");
	controls 	= document.getElementById("controls");
	info 		= document.getElementById("info");

	connect();
	initDatGUI();
	initMap();
}

function initDatGUI() {

	var voyage = new Voyage();
	var datGui = new dat.GUI();

	var timestep 		= datGui.add(voyage, 'timestep').min(0);
	var movement_factor = datGui.add(voyage, 'movement_factor');
	var range = datGui.add(voyage, 'range').min(0);
	var alpha = datGui.add(voyage, 'alpha');
	var altitude = datGui.add(voyage, 'altitude').min(0);
	var sail_open = datGui.add(voyage, 'sail_open');

	timestep.onFinishChange(function(value) {
  		send("timestep= " + value);
	});
	movement_factor.onFinishChange(function(value) {
		send("movement_factor= " + value);
	});
	range.onFinishChange(function(value) {
		send("range= " + value);
	});
	alpha.onFinishChange(function(value) {
		send("alpha= " + value);
	});
	altitude.onFinishChange(function(value) {
		send("altitude= " + value);
	});
	sail_open.onFinishChange(function(value) {
		send("sail_open= " + (value ? 1 : 0));
	});

	datGui.add(voyage, 'Run');

	controls.appendChild(datGui.domElement);
	datGui.close();
}
