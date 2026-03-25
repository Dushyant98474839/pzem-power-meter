#include <WiFi.h>
#include <WebServer.h>
#include <PZEM004Tv30.h>
#include <ESPmDNS.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";

PZEM004Tv30 pzem(Serial2, 16, 17);
WebServer server(80);

float voltage = 0;
float current = 0;
double power = 0;

bool measuring = false;
int ms_epoch=0;

double total_power = 0;
unsigned long lastSampleTime = 0;

void handleRoot() {

  String page = R"rawliteral(
  <!DOCTYPE html>
  <html>
      <head>
        <title>PZEM Monitor</title>
          <style>
          body{font-family:Arial;text-align:center;background:#111;color:#0f0}
          .card{font-size:30px;margin:20px}
          button{padding:10px 20px;font-size:18px}
          </style>
      </head>

      <body>

      <h2>PZEM Live Monitor</h2>

      <div class="card">Voltage: <span id="voltage">--</span> V</div>
      <div class="card">Current: <span id="current">--</span> A</div>
      <div class="card">Power: <span id="power">--</span> W</div>

      <button onclick="start()">Start Measurement</button>
      <button onclick="stop()">Stop Measurement</button>

  <script>

  function updateData(){

  fetch('/data')
  .then(response => response.json())
  .then(data => {

  document.getElementById("voltage").innerHTML = data.voltage.toFixed(2);
  document.getElementById("current").innerHTML = data.current.toFixed(3);
  document.getElementById("power").innerHTML = data.power.toFixed(2);

  });

  }

  function start(){
fetch('/start?epoch=200');
}

  function stop(){

  fetch('/stop')
  .then(response => response.json())
  .then(data => {

  alert("Power: " + data.total_power + " W");

  });

  }

  setInterval(updateData,1000);

  </script>

  </body>
  </html>
  )rawliteral";

  server.send(200,"text/html",page);
}

void handleStart() {

  // energyWh = 0;
  measuring = true;
  lastSampleTime = millis();
  if(server.hasArg("epoch")){
    ms_epoch = server.arg("epoch").toInt();
  }else{
      ms_epoch = 200;
  }
  server.send(200, "application/json",
              "{\"status\":\"measurement_started\"}");
}


void handleStop() {

  measuring = false;

  String response = "{";
  response += "\"total_power\":" + String(total_power,6);
  response += "}";

  server.send(200, "application/json", response);
}

void handleData() {

  String json = "{";
  json += "\"voltage\":" + String(voltage) + ",";
  json += "\"current\":" + String(current) + ",";
  json += "\"power\":" + String(power) + ",";
  json += "\"total_power\":" + String(total_power,6);
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {

  Serial.begin(115200);
  Serial2.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp32")) {
    Serial.println("Error starting mDNS");
  } else {
    Serial.println("mDNS started: http://esp32.local");
  }

  server.on("/", handleRoot);
  server.on("/start", handleStart);
  server.on("/stop", handleStop);
  server.on("/data", handleData);

  server.begin();
}

void loop() {

  server.handleClient();

  voltage = pzem.voltage();
      current = pzem.current();
      power   = pzem.power();
  unsigned long now = millis();

  unsigned long elapsedtime = now - lastSampleTime;
  if (measuring && elapsedtime >= ms_epoch) {

      float dt = elapsedtime / 1000.0;
      lastSampleTime = now;

      voltage = pzem.voltage();
      current = pzem.current();
      power   = pzem.power();

      if(!isnan(power)){

          

          total_power += power;

      }

      Serial.print("Voltage: ");
      Serial.println(voltage);

      Serial.print("Current: ");
      Serial.println(current);

      Serial.print("Power: ");
      Serial.println(power);

      // Serial.print("Energy: ");
      // Serial.println(energyWh);

      Serial.println("------------------------------------------------");
  }

}
