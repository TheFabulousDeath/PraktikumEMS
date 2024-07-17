#include "isensor.h"
#include "ible.h"

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
  BLEsetup();
  Serial.println("SETUP COMPLETE");
}

void loop() {
  magnetometerInterface->loop();
  activityInterface->loop();



  switch (state) {
    case calibration:
      stateString = "Kalibrieren";
      if(t0()) {state = start;}
      break;
    case start:
      stateString = "Start";
      if(t1()) {state = outside;}
      break;
    case outside:
      stateString = "Draußen";
      if(t3()) {state = inside;}
      break;
    case inside:
      stateString = "Drinnen";
      if(t4()) {state = outside;}
      if(t5()) {state = stairs;}
      if(t6()) {state = elevator;}
      break;
    case stairs:
      stateString = "Treppe";
      if(t8()) {state = inside;}
      break;
    case elevator:
      stateString = "Fahrstuhl";
      if(t7()) {state = inside;}  
      break;
  }

  sendState(&(activityInterface->storey));
  Serial.println("State: " + String(state) + ", OutsideScore" + String(magnetometerInterface->isOutside()) + ", IsWalking" + String(activityInterface->is_moving()) + "Floor: " + String(activityInterface->FD()));
}

bool t0() {
  // Fires when all sensors are calibrated
  return (magnetometerInterface->isCalibrated() && activityInterface->isCalibrated());
}

bool t1(){
  int outsideScore = magnetometerInterface->isOutside();
  return (outsideScore >= 90);
}

bool t3(){
  int outsideScore = magnetometerInterface->isOutside();
  return (outsideScore < 40);
}

bool t4(){
  int outsideScore = magnetometerInterface->isOutside();
  return (outsideScore >= 80);
}
bool t5(){
  return (activityInterface->is_moving() && activityInterface->vert_movement);
}
bool t6(){
  return (!( activityInterface->is_moving() ) && activityInterface->vert_movement);
}
bool t7(){
  return activityInterface->is_moving() && !(activityInterface->vert_movement);
}
bool t8(){
  return activityInterface->is_moving() && !(activityInterface->vert_movement);
}
