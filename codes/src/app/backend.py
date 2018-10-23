import sys
import time
import json
import paho.mqtt.client

# Subscriber
APPLICATION_ID = "centraltcc"
ACCESS_KEY = "ttn-account-v2.D_dLPhABWJO_VgI-jRMl9qyIeVKmvsRKhaXfzbfn3S8"
TOPIC = "centraltcc/devices/sinknode/up"
HOSTNAME = "brazil.thethings.network"
PORT = 1883
TIMEOUT = 60
client = paho.mqtt.client.Client(client_id='ttn_client', clean_session=False)

# Publisher
WEBSOCKET_HOST = "broker.hivemq.com"
WEBSOCKET_PORT = 8000
WEBSOCKET_TOPIC = "vagas/cpd/"
websocket_client = paho.mqtt.client.Client(client_id='websocket_client', transport="websockets")

def on_connect_websocket(websocket_client, userdata, flags, rc):
	print('connected (%s)' % websocket_client._client_id)
	
def on_connect(client, userdata, flags, rc):
	print('connected (%s)' % client._client_id)
	client.subscribe(topic=TOPIC, qos=2)

def send_data_to_app(data="hello world"):
    print("enviando: ", websocket_client.publish(WEBSOCKET_TOPIC, payload=data))

def on_message(client, userdata, message):
	PAYLOAD = message.payload
	print('------------------------------')
	print('topic: %s' % message.topic)
	print('payload: %s' % PAYLOAD)
	print('payload_type:' + str(type(PAYLOAD)))
	print('qos: %d' % message.qos)
    
	# Envia para aplicação
	data = json.loads(PAYLOAD.decode("utf-8"))["payload_raw"]
	print("#### ENVIANDO: " + str(data) + " ####")
	if str(data).__eq__("AA=="):
		data = "0"
	elif str(data).__eq__("AQ=="):
		data = "1"
	else:
		data = "2"

	send_data_to_app(data)
	
def main():
	# Cria e configura cliente que obtém dados da TTN
	client.username_pw_set(APPLICATION_ID, ACCESS_KEY)
	client.on_connect = on_connect
	client.on_message = on_message
	client.connect(HOSTNAME, PORT, TIMEOUT)

	# Cria e configura cliente mqtt para enviar dados ao front end
	websocket_client.on_connect = on_connect_websocket
	websocket_client.connect(WEBSOCKET_HOST, WEBSOCKET_PORT, TIMEOUT)
	
	# Inicia clientes
	websocket_client.loop_start()
	client.loop_start()
	
	# Aguarda mensagens
	while True:
		pass
	
	# Encerra clientes
	client.loop_stop()
	client.disconnect()
	websocket_client.loop_stop()
	websocket_client.disconnect()

if __name__ == '__main__':
	main()
	sys.exit(0)
