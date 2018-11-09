// Variáveis
var num_vagas_disponiveis = "0";
var marker;

// Instancia um cliente Paho MQTT
HOST = "broker.hivemq.com"
PORT = 8000
CLIENT_ID = "frontend_test"
client = new Paho.MQTT.Client(HOST, PORT, CLIENT_ID)

// Define as configurações de conexão
var connectionOptions = {
  keepAliveInterval: 20,
  timeout: 3,
  onSuccess: onConnect
}
client.connect(connectionOptions);

// Método chamado quando o cliente se conectar ao broker
function onConnect() {
  // Conexão realizada, então se inscreve no tópico desejado
  console.log("Iniciando...");
  marker.setLabel("?");
  
  var topic = "vagas/cpd/"
  client.subscribe(topic);
  console.log("Inscrito em: " + topic)
}

// Método chamado quando chegam novas informações sobre as vagas monitoradas
client.onMessageArrived = function (message) {
  console.log("onMessageArrived:" + message.payloadString);
  marker.setLabel(message.payloadString);
}

// Método chamado quando o cliente perde conexão com o broker
client.onConnectionLost = function (responseObject) {
  marker.setLabel("123");
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

// Inicializa mapa
function initMap() {
  var coords = { lat: -29.720966, lng: -53.714431};
  var map = new google.maps.Map(document.getElementById('map'), {
    zoom: 18,
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

