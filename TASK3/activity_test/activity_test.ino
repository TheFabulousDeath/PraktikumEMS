#include <cstdio>
#include <cstdlib>
#include <stdbool.h>

#include "Nicla_System.h"
#include <Arduino_BHY2.h>

SensorActivity activity(SENSOR_ID_AR);
Sensor baro(SENSOR_ID_BARO);

// Global variables:
enum motion {Undifined = -1, Still, NotStill, Moving} movement = Undifined;

int storey = 0;   // starting floor = 0
int storey_baro;   // pressure at starting floor
int counter = 0; 
int last_baro[8];
int avg_baro;   // to return last calculated avg



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
  
  storey_baro = avg_baro = baro_cal(10, 20);  // (#discarded, #base_average)
  
    
  Serial.println("Activity:");
}


int AR() {
  switch(activity.value()) {
    case 256: if(movement != Still) {
      movement = Still;
      //Serial.println(movement); 
      }
      break;
    case 1:
    case 8226:
    case 36: if(movement != NotStill) {
      movement = NotStill;
      //Serial.println(movement); 
      }
      break;
    case 512:
    case 1024: if(movement != movement) {
      movement = movement;
      //Serial.println(movement); 
      }
      break;    
  }

  return movement;
}

int average(int array[]) {
  int sum = 0;
  int l = sizeof(array) / sizeof(array[0]);
    
  for(int i = 0; i < l; i++) sum += array[i];
  
  return sum / l;
}

int AB() {
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
        counter++;
        break;
      case 4: last_baro[4] = baro_value();
        counter++;
        break;
      case 5: last_baro[5] = baro_value();
        counter++;
        break;
      case 6: last_baro[6] = baro_value();
        counter++;
        break;
      case 7: last_baro[7] = baro_value();
        Serial.println("Stry_baro: "+ String(storey_baro)+ "  Avg: " + String(average(last_baro)));
        counter = 0;
        avg_baro = average(last_baro);
        storey = FD();
        break;
  }
  
  return avg_baro;
}

int FD() {  // threashold = 34
  
}

bool approx(int value, int compare, int deviation) {
  return (value)
}

void loop() {
  static auto lastCheck = millis();

  if(millis()-lastCheck >= 100){
    lastCheck = millis();
    BHY2.update();

    

  AB();
//    Serial.println(String(FD())); 
  
  }
}
