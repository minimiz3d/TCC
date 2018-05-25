# import time
# import ttn

# app_id = "centraltcc"
# access_key = "ttn-account-v2.D_dLPhABWJO_VgI-jRMl9qyIeVKmvsRKhaXfzbfn3S8"


# def uplink_callback(msg, client):
#   print("Received uplink from ", msg.dev_id)
#   print(msg)


# handler = ttn.HandlerClient(app_id, access_key)

# # using mqtt client
# mqtt_client = handler.data()
# mqtt_client.set_uplink_callback(uplink_callback)
# mqtt_client.connect()
# time.sleep(60)
# mqtt_client.close()

# # using application manager client
# app_client = handler.application()
# my_app = app_client.get()
# print(my_app)
# my_devices = app_client.devices()
# print(my_devices)
import ssl
import sys

import paho.mqtt.client
import paho.mqtt.publish


APPLICATION_ID = "centraltcc"
ACCESS_KEY = "ttn-account-v2.D_dLPhABWJO_VgI-jRMl9qyIeVKmvsRKhaXfzbfn3S8"
TOPIC = "centraltcc/devices/sinknode/up"
HOSTNAME = "brazil.thethings.network"

def on_connect(client, userdata, flags, rc):
	print('connected')


def main():
	paho.mqtt.publish.single(
		topic=TOPIC,
		payload='uhulSSAS',
		qos=2,
		hostname=HOSTNAME,
		port=1883,
		client_id='uhul',
		auth={
			'username': APPLICATION_ID,
			'password': ACCESS_KEY
		},
		# tls={
		# 	'ca_certs': '/etc/ssl/certs/DST_Root_CA_X3.pem',
		# 	'tls_version': ssl.PROTOCOL_TLSv1_2
		# }
	)


if __name__ == '__main__':
	main()
	sys.exit(0)
