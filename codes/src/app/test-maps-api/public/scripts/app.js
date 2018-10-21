// Variáveis
var num_vagas_disponiveis = "0";
var marker;

// Create a client instance
HOST = "broker.hivemq.com"
PORT = 8000
CLIENT_ID = "arthur_teste"
client = new Paho.MQTT.Client(HOST, PORT, CLIENT_ID)

// connect the client
var connectionOptions = {
  keepAliveInterval: 20,
  timeout: 3,
  onSuccess: onConnect
}
client.connect(connectionOptions);


// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("entrou");
  marker.setLabel("on");
  
  var topic = "vagas/cpd/"
  client.subscribe(topic);
  console.log("Inscrito em: " + topic)
}

// called when the client loses its connection
client.onConnectionLost = function (responseObject) {
  marker.setLabel("123");
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

// called when a message arrives
client.onMessageArrived = function (message) {
  console.log("onMessageArrived:" + message.payloadString);
  marker.setLabel(message.payloadString);
}

// Inicializa mapa
function initMap() {
  var coords = { lat: -29.720415, lng: -53.7141605 };
  var map = new google.maps.Map(document.getElementById('map'), {
    zoom: 17,
    center: coords,
    scrollwheel: false
  });
  marker = new google.maps.Marker({
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

