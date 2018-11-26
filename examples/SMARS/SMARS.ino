#include"smars.h"

Smars smars;

void setup() {
 smars.begin();
}
void loop() {
  smars.move_forward(10);
  smars.move_left(5);
  smars.move_forward(10);
  smars.move_right(5);
  smars.move_back(10);
}
