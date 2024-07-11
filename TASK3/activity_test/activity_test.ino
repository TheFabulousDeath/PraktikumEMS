#include <cstdio>
#include <cstdlib>
#include <stdbool.h>

#include "Nicla_System.h"
#include <Arduino_BHY2.h>

SensorActivity activity(SENSOR_ID_AR);
Sensor baro(SENSOR_ID_BARO);

enum movement {Undifined = -1, Still, NotStill, Moving} active = Undifined;

int ground;
int counter = 0; //0-lengthoflast_baro
int last_baro[4]; // defines length of avg


int baro_value() {
  return int(baro.value() * 100);   // baro times 100
}


int baro_cal(int discarded, int values) {
  int avg = 0;

  for(int i =- discarded; i < values; ) {
      static auto lastCheck = millis();

    if(millis()-lastCheck >= 100){
      lastCheck = millis();
      BHY2.update();
      //Serial.println(baro.value());   
      if(i >= 0) avg += baro_value();
      i++;
    }
  }
  avg /= values;
  Serial.println("average: "+ String(avg));
  return avg;
}

void setup() {
  Serial.begin(115200);
  BHY2.begin();
  activity.begin();
  baro.begin();
  
  ground = baro_cal(10, 20);
    
  for(int i = 0; i < 4; i++) last_baro[i] = ground;
  //last_baro = {ground};
  Serial.println("Activity:");
}

void activity_print() {
  //Serial.println(active);
}

void AR() {
  switch(activity.value()) {
    case 256: if(active != Still) {
      active = Still;
      activity_print(); }
      break;
    case 1:
    case 8226:
    case 36: if(active != NotStill) {
      active = NotStill;
      activity_print(); }
      break;
    case 512:
    case 1024: if(active != Moving) {
      active = Moving;
      activity_print(); }
      break;    
  }
}

int average(int array[]) {
  int l = 4;//sizeof(array) / sizeof(array[0]);
  int sum = 0;
  
  for(int i = 0; i < 4; i++) sum += array[i];
  
  return sum / l;
}


void loop() {
  static auto lastCheck = millis();

  if(millis()-lastCheck >= 100){
    lastCheck = millis();
    BHY2.update();

    switch(counter) {
      case 0: last_baro[0] = baro_value();
        counter++;
        break;
      case 1: last_baro[1] = baro_value();
        counter++;
        break;
      case 2: last_baro[2] = baro_value();
        counter++;
        break;
      case 3: last_baro[3] = baro_value();
        Serial.println("Avg: " + String(average(last_baro)));
        counter = 0;
        break;
    }


    //Serial.println(String(baro_value())); 
  
  }
}
