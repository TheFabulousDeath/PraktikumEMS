#include "isensor.h"

bool t0(), t1(), t2(), t3(), t4(), t5(), t6(), t7(), t8();

enum STATES {calibration, start, outside, inside, stairs, elevator};
STATES state;

Magnetometer* magnetometerInterface = new Magnetometer();

void setup() {
  state = calibration;
  //Serial.println("Starting Magnetometer");
  magnetometerInterface->setup();
  Serial.println("SETUP COMPLETE");
}

void loop() {
  magnetometerInterface->loop();




  switch (state) {
    case calibration:
      if(t0()) {state = start;}
      break;
    case start:
      if(t1()) {state = outside;}
      break;
    case outside:
      if(t3()) {state = inside;}
      break;
    case inside:
      if(t4()) {state = outside;}
      if(t5()) {state = stairs;}
      if(t6()) {state = elevator;}
      break;
    case stairs:
      if(t8()) {state = inside;}
      break;
    case elevator:
      if(t7()) {state = inside;}  
      break;
  }
}

bool t0() {
  // Fires when all sensors are calibrated
  if(magnetometerInterface->isCalibrated() &&)
  return true;
}

bool t1(){
  return true;
}

bool t2(){
  return true;
}
bool t3(){
  return true;
}
bool t4(){
  return true;
}
bool t5(){
  return true;
}
bool t6(){
  return true;
}
bool t7(){
  return true;
}
bool t8(){
  return true;
}
