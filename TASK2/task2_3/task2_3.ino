#include "Nicla_System.h"

int buffer;
int input;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nicla::begin();
  nicla::leds.begin();
  Serial.println("ENSURE THAT THE SERIAL MONITOR DOES NOT SEND NEWLINE");
}

void loop() {
  // put your main code here, to run repeatedly:
  while ((buffer = Serial.available()) == 0) {
  }
  input = Serial.parseInt();
  Serial.println(input);
  switch(input) {
    case 0:
      nicla::leds.setColor(off);
      break;
    case 1:
      nicla::leds.setColor(red);
      break;
    case 2:
      nicla::leds.setColor(green);
      break;
    case 3:
      nicla::leds.setColor(blue);
      break;
    default:
      break;
  }
}
