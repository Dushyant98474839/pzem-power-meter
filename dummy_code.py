import requests
import time

ESP_IP = "http://192.168.137.151"   #ESP32 IP


def start_measurement():
    r = requests.get(f"{ESP_IP}/start")
    print("Measurement started:", r.json())


def stop_measurement():
    r = requests.get(f"{ESP_IP}/stop")
    print("Measurement stopped:", r.json())
    return r.json()["energy_Wh"]


def get_live_data():
    r = requests.get(f"{ESP_IP}/data")
    return r.json()



start_measurement()

print("Running heavy computation...")
time.sleep(5)

energy = stop_measurement()

print("Energy used:", energy, "Wh")

print("Current readings:", get_live_data())