#include <Arduino.h>
#include <move.h>

void setup()
{
  Serial.begin(115200);
  move_begin();
}

void loop()
{
  // set 100% PWM
  set_speed(PWMRANGE);
  // go straight
  move(STRAIGHT);
  delay(2000);
  // clockwise
  move(RIGHT);
  delay(2000);
  // anticlockwise
  move(LEFT);
  delay(2000);
  // reverse
  move(REVERSE);
  delay(2000);
  for (int slow_steps = 0; slow_steps <= 1000; slow_steps += 100)
  {
    set_speed(PWMRANGE - slow_steps);
    delay(300);
  }
  move(BRAKE);
  delay(2000);
}