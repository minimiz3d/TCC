"use strict";

var num_vagas_disponiveis = "0";

// Create a client instance
// client = new Paho.MQTT.Client(location.hostname, Number(location.port), "clientId");
var host = "m13.cloudmqtt.com";
var port = 11277;
var client = new Paho.MQTT.Client(host, port, "my_jsclient");

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
// client.connect({ onSuccess: onConnect });
var options = {
  useSSL: true,
  userName: "kcflxtae",
  password: "xnPQvjYVZmxQ",
  onSuccess: onConnect,
  onFailure: doFail,
  timeout: 3
}
client.connect(options);

// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConeect - success!");
  client.subscribe("/cloudmqtt");
  message = new Paho.MQTT.Message("Hello CloudMQTT");
  message.destinationName = "/cloudmqtt";
  client.send(message);
}

function doFail(e) {
  console.log("puta merda");
  console.log(e);
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  console.log("onMessageArrived:" + message.payloadString);
  num_vagas_disponiveis = Number(message.payloadString);
}

function initMap() {
  var coords = { lat: -29.720415, lng: -53.7141605 };
  var map = new google.maps.Map(document.getElementById('map'), {
    zoom: 17,
    center: coords,
    scrollwheel: false
  });
  var marker = new google.maps.Marker({
    title: "Vagas disponíveis",
    position: coords,
    animation: google.maps.Animation.DROP,
    label: num_vagas_disponiveis,
    map: map
  });
  marker.addListener('click', toggleBounce);
}

// Anima o 'marker'
function toggleBounce() {
  if (marker.getAnimation() !== null) {
    marker.setAnimation(null);
  } else {
    marker.setAnimation(google.maps.Animation.BOUNCE);
  }
}

