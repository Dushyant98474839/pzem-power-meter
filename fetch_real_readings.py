import requests
import time

ESP_IP = "http://192.168.24.159"   # ESP32 IP


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

print("Live readings:\n")

try:
    while True:
        data = get_live_data()

        voltage = data["voltage"]
        current = data["current"]
        power = data["power"]

        print(f"Voltage: {voltage:.2f} V | Current: {current:.2f} A | Power: {power:.2f} W")

        time.sleep(1)   # fetch every second

except KeyboardInterrupt:
    energy = stop_measurement()
    print("\nEnergy used:", energy, "Wh")