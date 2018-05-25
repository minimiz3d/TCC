import sys
import paho.mqtt.client

APPLICATION_ID = "centraltcc"
ACCESS_KEY = "ttn-account-v2.D_dLPhABWJO_VgI-jRMl9qyIeVKmvsRKhaXfzbfn3S8"
TOPIC = "centraltcc/devices/sinknode/up"
HOSTNAME = "brazil.thethings.network"
PORT = 1883
TIMEOUT = 60

# Cria cliente mqtt para enviar dados ao front end
USERNAME = "kcflxtae"
PASSWORD = "xnPQvjYVZmxQ"
CLOUDMQTT = "m13.cloudmqtt.com"
SSL_PORT = 31277
cloudmqtt_client = paho.mqtt.client.Client(client_id='uhul', clean_session=False)
cloudmqtt_client.username_pw_set(USERNAME, PASSWORD)
cloudmqtt_client.connect(CLOUDMQTT, SSL_PORT, TIMEOUT)
cloudmqtt_client.loop_start()

def on_connect(client, userdata, flags, rc):
	print('connected (%s)' % client._client_id)
	client.subscribe(topic=TOPIC, qos=2)

def send_data_to_cloudmqtt(data="hello world"):
    print("pub:", cloudmqtt_client.publish("testing", payload="hello world", qos=2, retain=True))

def on_message(client, userdata, message):
	print('------------------------------')
	print('topic: %s' % message.topic)
	print('payload: %s' % message.payload)
	print('qos: %d' % message.qos)
    send_data_to_cloudmqtt()

def main():
	client = paho.mqtt.client.Client(client_id='uhul', clean_session=False)
	client.username_pw_set(APPLICATION_ID, ACCESS_KEY)
	client.on_connect = on_connect
	client.on_message = on_message
	client.connect(HOSTNAME, PORT, TIMEOUT)
    
	client.loop_forever()


if __name__ == '__main__':
	main()
	sys.exit(0)
