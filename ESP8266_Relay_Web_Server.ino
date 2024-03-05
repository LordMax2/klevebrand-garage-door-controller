#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "WIFI_SSID"
#define STAPSK "WIFI_PASSWORD"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "<!DOCTYPE html /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" /><meta charset=\"UTF-8\"><title>Garage Door Controller | Trest</title><style>    * {        box-sizing: border-box;        font-family: \"SEGOE UI\", SANS-SERIF;    }    body {        margin:0;        background-image:url('https://img.freepik.com/free-vector/white-abstract-wallpaper_23-2148808300.jpg');        background-size: cover;    }    .container {        text-align: center;        width: 80%;        margin: auto;        max-width: 500px;        height:100vh;    }    .button-container {        display: flex;        flex-direction: row;        flex-wrap: wrap;        justify-content: space-evenly;    }    .button {        width: 45%;        padding: 2em;        margin: 5px 0;        cursor: pointer;        transition: 0.1s all;        min-width: 225px;    }    .button:hover {        transform: scale(0.96);    }    .border-top {        border-top: 1px solid black !important;    }    .footer {        position: absolute;        bottom:0;        left:0;        right:0;        margin:auto;        padding:1em 0;        width: 80%;    }    .footer a {        color:black;        text-decoration: none;    }    .blue-button-theme {        background-color: #2196f3 !important;        font-weight: 600 !important;        color: #FFF !important;    }</style><div class=\"container\">    <br />    <br />    <h1>Garage Door Control</h1>    <div class=\"button-container\"><a class=\"button blue-button-theme\" id=\"16\">Garageport 1</a> <a class=\"button blue-button-theme\" id=\"5\">Garageport 2</a> <a class=\"button blue-button-theme\" id=\"4\">Garageport 3</a> <a class=\"button blue-button-theme\" id=\"14\">Garageport 4</a></div>    <div class=\"border-top footer\">        <p>© <span id=\"year\">2024</span> Trest | Gränssnittet är gjord av <b><a href=\"https://trest.se\">Trest</a></b></p>    </div></div><script>    document.getElementById(\"year\").innerHTML = new Date().getFullYear();    document.querySelectorAll(\".button\").forEach((element) => {        element.addEventListener(\"click\", async (event) => {            var response = await fetch(`/gpio?pin=${event.target.id}`, { method: \"POST\" });            if (response.ok) {            } else {            }        });    });</script>");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleGPIO() {
  digitalWrite(led, 1);

  if(server.method() != HTTP_POST) {
    server.send(405, "text/plain", "");
    return;
  }

  int a = server.arg("pin").toInt();
  Serial.println(a);

  digitalWrite(a, LOW);
  delay(500);
  digitalWrite(a, HIGH);

  server.send(200, "text/plain", "");
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);

  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(14, OUTPUT);
  
  digitalWrite(16, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(14, HIGH);

  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("garaget")) { Serial.println("MDNS responder started"); }

  server.on("/", handleRoot);
  server.on("/gpio", handleGPIO);

  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
