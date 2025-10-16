import paho.mqtt.client as mqtt
import json
import time

BROKER_ADDRESS = "www.onenet.hk.chinamobile.com"
MQTT_PORT = 1883
CLIENT_ID = "tc201-1"
USERNAME = "N0aH1wbj5E"
PASSWORD = "version=2018-10-31&res=products%2FN0aH1wbj5E%2Fdevices%2Ftc201-1&et=2524579200&method=md5&sign=7q7J3DUMgRHNPYWpE7xAJA%3D%3D"


PRODUCT_ID = USERNAME
DEVICE_NAME = CLIENT_ID.split('-')[0] + '-' + CLIENT_ID.split('-')[1]
TOPIC = f"$sys/{PRODUCT_ID}/{DEVICE_NAME}/thing/property/post"


def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("成功连接")
    else:
        print(f"连接失败,返回码:{rc}\n")

def on_publish(client, userdata, mid):
    print(f"{mid}发布成功")


if __name__ == '__main__':
    client = mqtt.Client(client_id=CLIENT_ID)
    client.username_pw_set(username=USERNAME, password=PASSWORD)
    client.on_connect = on_connect
    client.on_publish = on_publish
    client.connect(BROKER_ADDRESS, MQTT_PORT, 60)

    data_to_send = {
        "id": "1",
        "version": "1.0",
        "params": {
            "Voltage": {
                "value": 220.5,
                "time": 1760511842000
            },
            "Current": {
                "value": 5.0,
                "time": 1760511842000
            }
        }
    }
    payload = json.dumps(data_to_send)
    client.loop_start()
    time.sleep(1)

    if client.is_connected():
        result = client.publish(TOPIC, payload)
        result.wait_for_publish()

        time.sleep(2)
    else:
        print("客戶端未連接")

    client.loop_stop()
    client.disconnect()
    print("\n已断开")
