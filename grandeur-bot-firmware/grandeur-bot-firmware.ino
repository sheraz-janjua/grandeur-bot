#include <Grandeur.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "move.h"
// Device's connection configurations
String apiKey = "grandeurkv7jxe7700af0k178y0f1xcf";
String deviceID = "devicekvrmkbrf0a9m0ixff2hj4phy";
String token = "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltUmxkbWxqWld0MmNtMXJZbkp4TUdFNWJqQnBlR1k1WmpSc01tUXliaUlzSW5SNWNHVWlPaUprWlhacFkyVWlMQ0pwWVhRaU9qRTJNelkwTXpRd01qRjkuRzZsUFljVTVqUVk3UGpBR21PV3BNNlpiaF9iQ0Z5dlVlX0lJOFRIWTYtbyJ9";
const char *ssid = "Octopus";
const char *passphrase = "Sheraz81";
motion lr[2] = {HALT, HALT};

// Object of Grandeur project.
Grandeur::Project project;

// Starts the device WiFi.
void startWiFi(void);
void cb(const char* path, const char* state) {
  bool param = path[0] == 'l';
  switch (state[0]) {
    case 'F': lr[param] = FORWARD; break;
    case 'R': lr[param] = BACKWARD; break;
    default: lr[param] = HALT; break;
  }
}void cb_speed(const char* path, const char* state) {
  String speed_str = String(state);
  uint16_t _speed = speed_str.toInt();
  switch (path[1]) {
    case 'l': set_speed_l(_speed); break;
    case 'r': set_speed_r(_speed); break;
    default: break;
  }
}
void cb_roll(const char* path, const char* state) 
{
  switch (state[0]) {
    case 'F': {
        lr[0] = FORWARD;
        lr[1] = FORWARD;
        break;
      }
    case 'R': {
        lr[0] = BACKWARD;
        lr[1] = BACKWARD;
        break;
      }
    default: {
        lr[0] = HALT;
        lr[1] = HALT;
        break;
      }
  }
}
void setup()
{
  Serial.begin(115200);
  move_begin();
  set_speed(PWMRANGE);
  startWiFi();
  // This initializes the SDK's configurations and returns a new object of Project class.
  project = grandeur.init(apiKey, token);
  Serial.println("Begin");
  project.device(deviceID).data().on("l", cb);
  project.device(deviceID).data().on("r", cb);
  project.device(deviceID).data().on("sl", cb_speed);
  project.device(deviceID).data().on("sr", cb_speed);
  project.device(deviceID).data().on("f", cb_roll);
}

void loop()
{
  // This runs the SDK only when the WiFi is connected.
  if (WiFi.status() == WL_CONNECTED) project.loop();
  move(lr[1], lr[0]);
}

void startWiFi(void)
{
  // Disconnecting WiFi if it"s already connected
  WiFi.disconnect();
  // Setting it to Station mode which basically scans for nearby WiFi routers
  //  WiFi.mode(WIFI_STA);
  // Begin connecting to WiFi
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid, passphrase);
  // Device program gets in the while loop as long as the WiFi isn't connected.
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("*");
    delay(1000);
  }
  Serial.println("*");
  // This gets printed after the WiFi is connected.
  Serial.printf("\nDevice has successfully connected to WiFi. Its IP Address is: %s\n", WiFi.localIP().toString().c_str());
}
