#include "ThingsBoard.h"
#include <ESP8266WiFi.h>


#define WIFI_AP             "Foysal"   // name of your wifi
#define WIFI_PASSWORD       "asdfghjkl" // password of your wifi

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "wTp7r5aSTYFgoKblzcx4"  // enter access token of your ThingsBoard Device
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200
int var = 1;

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
}

bool subscribed = false;

RPC_Response ts1(const RPC_Data &data)
{
  Serial.println("Received the set switch method 4!");
  char params[10];
  serializeJson(data, params);
  //Serial.println(params);
  String _params = params;
  if (_params == "true") {
    Serial.println("Toggle Switch - 1 => On");

  }
  else  if (_params == "false")  {
    Serial.println("Toggle Switch - 1 => Off");
  }
}


const size_t callbacks_size = 1;
RPC_Callback callbacks[callbacks_size] = {
  { "getValue_1",         ts1 }   // enter the name of your switch variable inside the string

};

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    subscribed = false;
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  if (!subscribed) {
    Serial.println("Subscribing for RPC...");

    // Perform a subscription. All consequent data processing will happen in
    // processTemperatureChange() and processSwitchChange() functions,
    // as denoted by callbacks[] array.
    if (!tb.RPC_Subscribe(callbacks, callbacks_size)) {
      Serial.println("Failed to subscribe for RPC");
      return;
    }

    Serial.println("Subscribe done");
    subscribed = true;
  }

  //Serial.println("Sending data...");

  //tb.sendTelemetryInt("temperature", var);
  //var ++;

  tb.loop();
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
