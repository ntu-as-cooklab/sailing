var ws;
var url;

function connect() {
	url = "ws://127.0.0.1";

	if ("WebSocket" in window)
		ws = new WebSocket(url);
	else if ("MozWebSocket" in window)
		ws = new MozWebSocket(url);
	/*else
		return document.getElementById("console").innerHTML += "This browser does not support WebSocket<br/>";

	ws.onopen = function(e) {
		document.getElementById("console").innerHTML += "[Client] A connection to "+ws.url+" has been opened.<br/>";
		document.getElementById("server_url").disabled = true;
		document.getElementById("toggle_connect").innerHTML = "Disconnect";
		document.getElementById("send").disabled = false;
	};

	ws.onerror = function(e) {
		document.getElementById("console").innerHTML += "[Client] An error occured, see console log for more details.<br/>";
		console.log(e);
	};

	ws.onclose = function(e) {
		document.getElementById("console").innerHTML += "[Client] The connection to "+url+" was closed. ["+e.code+(e.reason != "" ? ","+e.reason : "")+"]<br />";
	    cleanup_disconnect();
	};

	ws.onmessage = function(e) {
		document.getElementById("console").innerHTML += "[Server] "+e.data+"<br />";
		$("#container").scrollTop($("#container")[0].scrollHeight);
	};

	document.onkeydown = function(){
	    if(window.event.keyCode=='13')
	        send();
	}
	document.getElementById("msg").focus();*/
}

function disconnect() {
	ws.close();
	cleanup_disconnect();
}

function cleanup_disconnect() {
    //document.getElementById("server_url").disabled = false;
	//document.getElementById("toggle_connect").innerHTML = "Connect";
	//document.getElementById("send").disabled = true;
}

function toggle_connect() {
	//if (document.getElementById("server_url").disabled === false) {
	//	connect();
	//} else {
	//	disconnect();
	//}
}

function send(msg) {
	ws.send(msg);
	//document.getElementById("console").innerHTML += "[Client] Send message: " + msg +"<br />";
}
