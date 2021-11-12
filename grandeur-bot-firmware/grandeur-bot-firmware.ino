#include <Grandeur.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "move.h"
#include "math.h"
// Device's connection configurations
String apiKey = "grandeurkv7jxe7700af0k178y0f1xcf";
String deviceID = "devicekvvxu41q00260pxfcat23jum";
String token = "eyJ0b2tlbiI6ImV5SmhiR2NpT2lKSVV6STFOaUlzSW5SNWNDSTZJa3BYVkNKOS5leUpwWkNJNkltUmxkbWxqWld0MmRuaDFOREY1TURBeU56QndlR1l4Y0dFd00yc3phQ0lzSW5SNWNHVWlPaUprWlhacFkyVWlMQ0pwWVhRaU9qRTJNelkyT1RRNE1UaDkuZHlGOWZKUUhfRXdCNl82NVp4b3pfeWN6bkNXeE9pN1F3TkdZclYzNGMwVSJ9";
const char *ssid = "Octopus";
const char *passphrase = "Sheraz81";
motion lr[2] = {HALT, HALT};

// Object of Grandeur project.
Grandeur::Project project;

// Starts the device WiFi.
void startWiFi(void);
void cb(const char *path, const char *state)
{
  // TODO: handle joystick x,y
  // 1- decode x y
  String xy_str = String(state);
  unsigned char comma_pos = xy_str.indexOf(',');
  String x_str = xy_str.substring(0, comma_pos);
  String y_str = xy_str.substring(comma_pos + 1);
  //Serial.printf("\n%s:%d:(%s,%s)\n", xy_str, comma_pos, x_str, y_str);
  int _x = x_str.toInt();
  int _y = y_str.toInt();

  // 2- decode f
  for (int param = 0; param < 2; param++)
    lr[param] = (_y > 0) ? BACKWARD : (_y < 0) ? FORWARD
                : HALT;
  // 3- decode speeds
  float speed = round(sqrt(pow(_x, 2) + pow(_y, 2))/10);
  uint16_t sl = (_x > 0) ? _x * speed : (_x < 0) ? (100 + _x) * speed : speed;
  uint16_t sr = (_x > 0) ? (100 - _x) * speed : (_x < 0) ? (-1 * _x * speed) : speed;
//  Serial.printf("\n%d,%d:%f:%d,%d\n", _x, _y, speed, sl, sr);
  set_speed_l(sl);
  set_speed_r(sr);
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
  project.device(deviceID).data().on("p", cb);
}

void loop()
{
  // This runs the SDK only when the WiFi is connected.
  if (WiFi.status() == WL_CONNECTED)
    project.loop();
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
