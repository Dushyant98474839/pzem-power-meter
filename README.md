# PZEM004T-ESP32 Power Meter

## I. Circuit Diagram

**Components:**
1. [Extension-Cord (3 core 2.5 Sq.mm Wire, Socket & Plug 16 Amp)](https://www.amazon.in/White-Cloth-Wire-16A-Socket-10Mtr/dp/B0D6FLQ6QQ?th=1)
2. [Connectors (2 units of these)](https://robu.in/product/pct-spl-42-0-08-2-5mm-42-pole-wire-connector-terminal-block-with-spring-lock-lever-for-cable-connection/?gad_source=1&gad_campaignid=17427802703&gbraid=0AAAAADvLFWfsKr6vwLElFZMt8mqGw-Ffi&gclid=Cj0KCQiAtfXMBhDzARIsAJ0jp3CsetmY617wdUMyLswsCoNAOmeStkDwQ39hzjyOCdQJbruxoBKX-P8aAnQ2EALw_wcB)
3. [ESP-WROOM-32 WIFI Bluetooth Networking Smart Component Development Board](https://robu.in/product/esp-wroom-32-wifi-bluetooth-networking-smart-component-development-board/?gad_source=1&gad_campaignid=17413441824&gbraid=0AAAAADvLFWe07-GBix7SMWnGKGcS3JAod&gclid=Cj0KCQiAtfXMBhDzARIsAJ0jp3AaROeVWXqo5jXzJOtBqqJtWUPMMymO0DkZ-N29NbkwqKVAOn344AUaAs-zEALw_wcB#)
4. [PZEM-004T (v3) 100A](https://roboticsdna.in/product/pzem-004t-3-0-version-communication-module-ttl-modbus-rtu-ac-single-phase-100a/?srsltid=AfmBOoqNxbgvD5HMbBEu7d-gEFH3hXqUSANVwiQf2mZRwdRrm-SkPyRujeE)

**Canva:**
[https://www.canva.com/design/DAHE-KQw0kc/tS8U1siF8ThDMMetPkLH8Q/edit?utm_content=DAHE-KQw0kc&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton](https://www.canva.com/design/DAHE-KQw0kc/tS8U1siF8ThDMMetPkLH8Q/edit?utm_content=DAHE-KQw0kc&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton)

<img src="https://raw.githubusercontent.com/Dushyant98474839/Pzem-Power-Meter/main/CircuitDiagram.png" width="800"/>

*Fig: Circuit Diagram of the PZEM004T-ESP32 Power Meter*

---

### Connection Flow

1. **AC Mains → Connector 1**
   - Live (Red)
   - Neutral (Blue)
   - Ground (Yellow)

2. **From Connector 1, Live wire splits into two paths:**
   - Path 1 → Goes to PZEM Voltage Input (Live)
   - Path 2 → Passes through Current Transformer (CT) → Goes to Connector 2 → Load (Socket Live)

3. **From Connector 1, Neutral wire splits into two paths:**
   - Path 1 → Goes to PZEM Voltage Input (Neutral)
   - Path 2 → Goes directly to Connector 2 → Load (Socket Neutral)

4. **Ground wire**
   - Goes directly from Connector 1 → Connector 2 → Load Ground

5. **PZEM to ESP32 (UART Communication):**
   - PZEM TX → ESP32 GPIO16 (RX2)
   - PZEM RX → ESP32 GPIO17 (TX2)
   - PZEM VCC → ESP32 5V
   - PZEM GND → ESP32 GND

---

## II. Flash ESP32

### 1. Install ESP32 Board in Arduino IDE

1. Open Arduino IDE
2. Go to **File → Preferences**
3. In **Additional Board Manager URLs**, paste:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to **Tools → Board → Boards Manager**
5. Search **ESP32**
6. Install **ESP32 by Espressif Systems**

### 2. Install Required Libraries

Go to **Sketch → Include Library → Manage Libraries**, install: `PZEM004Tv30.h` by Jakub

### 3. Select ESP32 Board and Port

- **Tools → Board → ESP32 Dev Module**
- **Tools → Port → Select COM Port**

### 4. Upload Code to ESP32

1. Paste code into Arduino IDE
2. Set Wifi-SSID and Password.
3. Click **Upload**
4. If upload fails, hold **BOOT** button on ESP32 while uploading
5. Wait until "Done Uploading"

### 5. Open Serial Monitor

- **Tools → Serial Monitor**
- Set **Baud Rate = 115200**
- You will see:

```
Connecting....
IP: 192.168.X.X
mDNS started: http://esp32.local
```

### 6. Open Webpage

```
http://<ESP32_IP_Address>
```

### 7. How It Works (Execution Steps)

1. ESP32 connects to WiFi
2. ESP32 starts web server
3. Webpage requests data from ESP32 every 1 second
4. ESP32 reads voltage, current, power from PZEM

### 8. Important Notes

- **ESP32 and PC/Phone must be on same WiFi network**
- PZEM baud rate = 9600
- Serial monitor baud rate = 115200
- UART used = Serial2 (GPIO16, GPIO17)
