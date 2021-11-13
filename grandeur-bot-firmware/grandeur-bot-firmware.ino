#include <Grandeur.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "move.h"
#include "math.h"

#define FS 1023 // full speed PWM
#define TS 50 // turn speed PWM

// Device's connection configurations
String apiKey = "grandeurkv7jxe7700af0k178y0f1xcf";
String deviceID = "devicekvxja2pn00fc0pxf1rds7uo3";
String token = "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltUmxkbWxqWld0MmVHcGhNbkZxTURCbVpEQndlR1l5YmpsdFlXVmhZU0lzSW5SNWNHVWlPaUprWlhacFkyVWlMQ0pwWVhRaU9qRTJNelkzT1RFek1EQjkuLWV0YUxLb1I4Y0RXbW1zNXpkLVdCOHZ1YTNpUXNSLUJrNUNvNXNMQlVfOCJ9";
const char *ssid = "Octopus";
const char *passphrase = "Sheraz81";
// Object of Grandeur project.
Grandeur::Project project;

// Starts the device WiFi.
void startWiFi(void);
void cb(const char *path, const char *state)
{
  motion lr[2] = {HALT, HALT};
  uint16_t sl = FS;
  uint16_t sr = FS;
  // TODO: handle joystick N,S,E,W,etc.
  // 1- decode N/S
  Serial.println(state);
  // 2- decode f
  for (int param = 0; param < 2; param++)
    lr[param] = (state[0] == 'S') ? BACKWARD : (state[0] == 'N')  ? FORWARD
                : HALT;
  // 3- decode speeds // full for N/S half the other side for turn
  switch (state[0]) {
    case 'N': if (state[1] == 'E')sr = TS; else if (state[1] == 'W')sl = TS; break;
    case 'S': if (state[1] == 'E')sr = TS; else if (state[1] == 'W')sl = TS; break;
    case 'E': lr[1] = FORWARD; break;
    case 'W': lr[0] = FORWARD; break;
    default: sl = 0; sr = 0; break;
  }
  set_speed_l(sl);
  set_speed_r(sr);
  move(lr[1], lr[0]);
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
  project.device(deviceID).data().on("D", cb);
}

void loop()
{
  // This runs the SDK only when the WiFi is connected.
  if (WiFi.status() == WL_CONNECTED)
    project.loop();
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
  Serial.print("* WiFI connected. IP address: ");
  Serial.println(WiFi.localIP());
}
