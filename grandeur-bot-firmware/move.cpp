#include "move.h"
void move_begin()
{
    pinMode(LFor, OUTPUT);
    pinMode(RFor, OUTPUT);
    pinMode(LBck, OUTPUT);
    pinMode(RBck, OUTPUT);
    pinMode(LPWM, OUTPUT);
    pinMode(RPWM, OUTPUT);
}
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
    if (value > PWMRANGE)
        value = PWMRANGE;
    analogWrite(LPWM, value);
    analogWrite(RPWM, value);
}
void set_speed_l(uint16_t value){
    if (value > PWMRANGE)
        value = PWMRANGE;
    analogWrite(LPWM, value);
}
void set_speed_r(uint16_t value){
    if (value > PWMRANGE)
        value = PWMRANGE;
    analogWrite(RPWM, value);
}
void move(movement m)
{
    switch (m)
    {
    case STRAIGHT:
    {
        move(FORWARD, FORWARD);
        break;
    }
    case REVERSE:
    {
        move(BACKWARD, BACKWARD);
        break;
    }
    case LEFT:
    {
        move(BACKWARD, FORWARD);
        break;
    }
    case RIGHT:
    {
        move(FORWARD, BACKWARD);
        break;
    }
    default:
    {
        move(HALT, HALT);
        break;
    }
    }
}
