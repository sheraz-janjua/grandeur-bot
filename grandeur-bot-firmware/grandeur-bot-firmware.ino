#include <Grandeur.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// L298N motor driver module pinout
// may need some swapping for different setups
#define LFor D3 // 0 IN4
#define RFor D4 // 2 IN1
#define LBck D1 // 5 IN3
#define RBck D2 // 4 IN2
#define LPWM D5 // 14
#define RPWM D6 // 12

#define FS 1023 // full speed PWM
#define TS 50 // diagonal turn speed PWM

// Cloud credentials
const char *apiKey = "---grandeur-cloud-project-api-key------";
const char *deviceID = "-device-id-created-by-grandeur-cloud-";
const char *token = "----token-generated-by-grandeur-cloud-on-device-registration-";
// Wi-Fi credentials
const char *ssid = "********";
const char *passphrase = "********";

// Object of Grandeur project.
Grandeur::Project project;

// enumeration for motor movement
enum motion
{
  HALT,
  FORWARD,
  BACKWARD
};

// function declarations

// Wi-Fi initialization
void startWiFi(void);

// callback for direction update
void cb(const char *path, const char *state);

// motor driver functions
// direction functions
void move(motion l, motion r);// both side motors at once
void move(motion m, uint8_t for_pin, uint8_t bck_pin);// each side individually
// speed functions
void set_speed_l(uint16_t value); //left wheel
void set_speed_r(uint16_t value); //right wheel
void set_speed(uint16_t value);   // both wheels
// initial motor driver output pins
void move_begin();

// configure/initialize
void setup()
{
  Serial.begin(115200); // to inform Wi-fi connection status
  // initialize the motor driver output pins
  move_begin(LFor, RFor, LBck, RBck, LPWM, RPWM);
  set_speed(FS); // full speed at start
  startWiFi();// initialize Wi-Fi
  // initialize the SDK
  project = grandeur.init(apiKey, token);
  // attach callback to variable update
  project.device(deviceID).data().on("D", cb);// D is the direction variable
}

void loop()
{
  // run SDK when Wi-Fi is connected.
  if (WiFi.status() == WL_CONNECTED)
    project.loop();// the Grandeur Cloud Arduino SDK runtime
}

// callback for direction update
void cb(const char *path, const char *state)
{
  motion lr[2] = {HALT, HALT}; // consider as left and right wheel movements
  uint16_t sl = FS, sr = FS;// left and right wheel speeds

  // handle joystick directions N,S,E,W,NE,NW,SE,SW.
  // and C is for stop
  // Serial.println(state);

  // evaluate forward or backward from first character
  for (int param = 0; param < 2; param++)// set direction of both wheels one-by-one
    lr[param] = (state[0] == 'S') ? BACKWARD : (state[0] == 'N')  ? FORWARD
                : HALT; // if(){} else if(){} else{} --- using ternary operator
  // diagonal directions NE/NW/SE/SW require slowing down one wheel
  // and E/W require running one wheel only
  switch (state[0]) {
    case 'N': if (state[1] == 'E')sr = TS; else if (state[1] == 'W')sl = TS; break;
    case 'S': if (state[1] == 'E')sr = TS; else if (state[1] == 'W')sl = TS; break;
    case 'E': lr[1] = FORWARD; break;
    case 'W': lr[0] = FORWARD; break;
    default: sl = 0; sr = 0; break;// for default case no speed no movement
  }
  // set the speeds on respective motors using PWM
  set_speed_l(sl);
  set_speed_r(sr);
  // execute the movement
  move(lr[1], lr[0]);
}

// initialize Wi-Fi
void startWiFi(void)
{
  WiFi.disconnect();
  WiFi.begin(ssid, passphrase);
  Serial.printf("\nConnecting to WiFi: %s\n", ssid);
  // Device program gets in the while loop as long as the WiFi isn't connected.
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(".");
  Serial.println("Connected");
}

// initialize outputs
void move_begin()
{
  pinMode(LFor, OUTPUT);
  pinMode(RFor, OUTPUT);
  pinMode(LBck, OUTPUT);
  pinMode(RBck, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(RPWM, OUTPUT);
}
//
void move(motion l, motion r)
{
  move(l, LFor, LBck);
  move(r, RFor, RBck);
}
void move(motion m, uint8_t for_pin, uint8_t bck_pin)
{
  bool for_out = (m == FORWARD);
  bool bck_out = (m == BACKWARD);
  digitalWrite(for_pin, for_out);
  digitalWrite(bck_pin, bck_out);
}

void set_speed(uint16_t value)
{
  if (value > FS)
    value = FS;
  analogWrite(LPWM, value);
  analogWrite(RPWM, value);
}
void set_speed_l(uint16_t value) {
  if (value > FS)
    value = FS;
  analogWrite(LPWM, value);
}
void set_speed_r(uint16_t value) {
  if (value > FS)
    value = FS;
  analogWrite(RPWM, value);
}
