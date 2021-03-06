import { ws } from "./websocket.js";

window.onload = function () {
	document.getElementById("auto-mode-button").addEventListener("click", (_) => sendConfigMessage("{'mode':'auto'}"));
	document
		.getElementById("manual-mode-button")
		.addEventListener("click", (_) => sendConfigMessage("{'mode':'manual'}"));
	document.getElementById("debug-send-button").addEventListener("click", (_) => sendCustomMessage());
	document.getElementById("download-tracking-file").addEventListener("click", (_) => window.location.href = "pof-lux/tracking");
	document.getElementById("clear-tracking-file").addEventListener("click", (_) => window.location.href = "pof-lux/clear_tracking");
};

function setOpMode(mode) {
	var buttons = document.getElementsByClassName("mode-button");
	for (var i = 0, len = buttons.length; i < len; i++) {
		buttons[i].style.backgroundColor = "gray";
	}
	document.getElementById(mode + "-mode-button").style.backgroundColor = "green";
}

ws.onmessage = function (response) {
	let json = JSON.parse(response.data);

	setOpMode(json["mode"]);
	document.getElementById("sun_position").innerHTML = json["sunPosition"];
	document.getElementById("manual_position").innerHTML = json["manualSetpoint"];
	document.getElementById("lens_angle").innerHTML = json["MPU"]["lensAngle"].toFixed(1);

	document.getElementById("rtc_day").innerHTML = json["RTC"]["day"];
	document.getElementById("rtc_month").innerHTML = json["RTC"]["month"];
	document.getElementById("rtc_year").innerHTML = json["RTC"]["year"];
	document.getElementById("rtc_hour").innerHTML = json["RTC"]["hour"];
	document.getElementById("rtc_minute").innerHTML = json["RTC"]["minute"];
	document.getElementById("rtc_second").innerHTML = json["RTC"]["second"];

	document.getElementById("motor_pwm").innerHTML = (json["motor"]["pwm"] / 2.55).toFixed(1) + "%";
	document.getElementById("motor_direction").innerHTML = json["motor"]["direction"];

	document.getElementById("kp").innerHTML = json["PID_values"]["kp"];
	document.getElementById("ki").innerHTML = json["PID_values"]["ki"];
	document.getElementById("kd").innerHTML = json["PID_values"]["kd"];
	document.getElementById("P").innerHTML = json["PID_values"]["p"].toFixed(1);
	document.getElementById("I").innerHTML = json["PID_values"]["i"].toFixed(2);
	document.getElementById("D").innerHTML = json["PID_values"]["d"].toFixed(2);
	document.getElementById("pid_output").innerHTML = json["PID_values"]["output"];
};

function sendCustomMessage() {
	let message = document.getElementById("debug-message-text-field").value;
	ws.send(message);
}
function sendConfigMessage(message) {
	ws.send(message);
}
