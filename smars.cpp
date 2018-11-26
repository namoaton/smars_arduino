#include "smars.h"

void Smars::begin() {
  Serial.begin(115200);
  Serial.flush();
  left_motor_dir = 13;
  right_motor_dir = 12;
  right_motor_pwm_pin = 10;
  left_motor_pwm_pin = 11;
  echo_pin = 8;
  trigger_pin = 7;
  pan_servo_pin = 9;
  buzzer = 4;
  default_pwm = 255;
  time_slot = 100;

  pinMode(buzzer, OUTPUT);
  pinMode(left_motor_dir, OUTPUT);
  pinMode(right_motor_dir, OUTPUT);
  pinMode(left_motor_pwm_pin, OUTPUT);
  pinMode(right_motor_pwm_pin, OUTPUT);
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
};

int Smars::get_distance()
{
  Serial.println("S230");
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  int duration = pulseIn(echo_pin, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2;
  if (distance < 0) {
    distance  = 0;
  }
  // Prints the distance on the Serial Monitor
  Serial.print("S231 ");
  Serial.println(distance);
  return distance;
};
void Smars::move_forward(int delay_slot) {
  analogWrite(right_motor_pwm_pin, default_pwm);
  analogWrite(left_motor_pwm_pin, default_pwm);
  digitalWrite(left_motor_dir, LOW);
  digitalWrite(right_motor_dir, LOW);
  STATUS = FW;
  Serial.print("S400 ");
  Serial.println(STATUS);
  delay(delay_slot * time_slot);
  stop_move();
};

void Smars::move_back(int delay_slot) {
  analogWrite(right_motor_pwm_pin, default_pwm);
  analogWrite(left_motor_pwm_pin, default_pwm);
  digitalWrite(left_motor_dir, HIGH);
  digitalWrite(right_motor_dir, HIGH);
  STATUS = BACK;
  Serial.print("S400 ");
  Serial.println(STATUS);
  delay(delay_slot * time_slot);
  stop_move();
};

void Smars::move_left(int delay_slot) {
  analogWrite(right_motor_pwm_pin, 0);
  analogWrite(left_motor_pwm_pin, default_pwm);
  digitalWrite(left_motor_dir, LOW);
  digitalWrite(right_motor_dir, LOW);
  STATUS = LEFT;
  Serial.print("S400 ");
  Serial.println(STATUS);
  delay(delay_slot * time_slot);
  stop_move();
};

void Smars::move_right(int delay_slot) {
  analogWrite(right_motor_pwm_pin, default_pwm);
  analogWrite(left_motor_pwm_pin, 0);
  digitalWrite(left_motor_dir, LOW);
  digitalWrite(right_motor_dir, LOW);
  STATUS = RIGHT;
  Serial.print("S400 ");
  Serial.println(STATUS);
  delay(delay_slot * time_slot);
  stop_move();
};

void Smars::stop_move() {
  analogWrite(right_motor_pwm_pin, 0);
  analogWrite(left_motor_pwm_pin, 0);
  digitalWrite(left_motor_dir, LOW);
  digitalWrite(right_motor_dir, LOW);
  STATUS = STOP;
  Serial.print("S400 ");
  Serial.println(STATUS);
};

void Smars::buzz(int slot) {
  Serial.print("S300 ");
  Serial.print("BUZZER ");
  Serial.println(slot * time_slot);
  digitalWrite(buzzer, HIGH);
  delay(slot * time_slot);
  digitalWrite(buzzer, LOW);
};

String Smars::parse_command(String msg) {
  char cmd[10] = {0};
  for (int i = 0; i < 10; i++) {
    cmd[i] = msg[i];
    if ( msg[i] == ' ') {
      cmd[i] = 0;
      break;
    }
    if (msg[i] == 's') {
      cmd[i] = 'S';
    }
  }
  return (String)cmd;
};

int Smars::get_command(String cmd) {
  if (cmd == "S101") {
    return 1;
  }
  if (cmd == "S102") {
    return 2;
  }
  if (cmd == "S103") {
    return 3;
  }
  if (cmd == "S104") {
    return 4;
  }
  if (cmd == "S105") {
    return 5;
  }
  if (cmd == "S231") {
    return 6;
  }
  if (cmd == "S237") {
    return 7;
  }
};

void Smars::control(String incomingMessage) {
  String cmd = parse_command(incomingMessage);
  int slot = incomingMessage.substring(4).toInt();
  Serial.println(cmd);
  Serial.println(slot);
  switch (get_command(cmd)) {
  case 1:
    move_forward(slot);
    break;
  case 2:
    move_back(slot);
    break;
  case 3:
    move_left(slot);
    break;
  case 4:
    move_right(slot);
    break;
  case 5:
    stop_move();
    break;
  case 6:
    get_distance();
    break;
  case 7:
    buzz(slot);
    break;
  default:
    break;
  }
};

void Smars::loop() {
  if (Serial.available() > 0) {
    String incomingMessage = Serial.readString();
    control(incomingMessage);
  }
};


