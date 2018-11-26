/*
  smars.h - Library for arduino SMARS robot.
  Created by Max Blashchuk, November 266, 2018.
  Released into the public domain.
*/
#ifndef Smars_h
#define Smars_h
#include "Arduino.h"
class Smars {
private:
  int left_motor_dir, right_motor_dir, right_motor_pwm_pin, left_motor_pwm_pin, echo_pin, trigger_pin, pan_servo_pin, buzzer, default_pwm, time_slot;
  int distance = 0 ;
  int STATUS;

  enum CURRENT_STATUS
  {
    STOP,
    FW,
    BACK,
    LEFT,
    RIGHT
  };
public:
  Smars() {};
  void begin();
  int get_distance();
  void move_forward(int delay_slot);
  void move_back(int delay_slot);
  void move_left(int delay_slot);
  void move_right(int delay_slot);
  void stop_move();
  void buzz(int slot);
  String parse_command(String msg);
  int get_command(String cmd);
  void control(String incomingMessage);
  void loop();
};
#endif
