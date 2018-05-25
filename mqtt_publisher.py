import paho.mqtt.client as mqtt
from random import randint

# SERVER = "m13.cloudmqtt.com"
# USER = "kcflxtae"
# PASSWORD = "xnPQvjYVZmxQ"
# PORT = 11277

USER = "centraltcc"
PASSWORD = "ttn-account-v2.D_dLPhABWJO_VgI-jRMl9qyIeVKmvsRKhaXfzbfn3S8"
TOPIC = "centraltcc/devices/sinknode/up"
SERVER = "brazil.thethings.network"
PORT = 1883


def on_connect(client, userdata, flags, rc):
	print('connected (%s)' % client._client_id)
	client.subscribe(topic=TOPIC, qos=2)

def on_message(client, userdata, message):
	print("received: ", str(message.payload.decode("utf-8")))
	print("topic: ", str(message.topic))


client = mqtt.Client(client_id='uhul', clean_session=False)
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(USER, PASSWORD)
client.connect(SERVER, PORT, 60)
print('subscribing to: ' + TOPIC)
client.subscribe(TOPIC)
client.loop_start()

print("pub:", client.publish(TOPIC, payload=str(randint(0,100)), qos=2, retain=True))

client.loop_stop()
client.disconnect()
