#include <Arduino.h>
// L298N pinout
#define LFor D3 // 0 IN4
#define RFor D4 // 2 IN1
#define LBck D1 // 5 IN3
#define RBck D2 // 4 IN2
#define LPWM D5 // 14
#define RPWM D6 // 12
enum movement
{
    BRAKE,
    STRAIGHT,
    REVERSE,
    LEFT,
    RIGHT
};

// enumeration for each motor
enum motion
{
    HALT,
    FORWARD,
    BACKWARD
};
// function declaration
// bot movement
void move(movement m);
// left right rotation direction
void move(motion l, motion r);
// motor driver output
void move(motion m, uint8_t for_pin, uint8_t bck_pin);
// set PWM for speed
void set_speed(uint16_t value);
// initial configuration
void move_begin();