#include "isensor.h"

bool t0(), t1(), t2(), t3(), t4(), t5(), t6(), t7(), t8();

enum STATES {calibration, start, outside, inside, stairs, elevator};
STATES state;

Magnetometer* magnetometerInterface = new Magnetometer();
Activity* activityInterface = new Activity();

void setup() {
  Serial.begin(115200);
  BHY2.begin();
  state = calibration;
  //Serial.println("Starting Magnetometer");
  magnetometerInterface->setup();
  activityInterface->setup();
  Serial.println("SETUP COMPLETE");
}

void loop() {
  magnetometerInterface->loop();
  activityInterface->loop();



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
  if(magnetometerInterface->isCalibrated() && activityInterface->isCalibrated()){return true;}
}

bool t1(){
  int outsideScore = magnetometerInterface->isOutside() + ;
  if(outsideScore >= 80) {return true;}
  return false;
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
