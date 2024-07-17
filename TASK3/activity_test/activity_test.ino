#include <cstdio>
#include <cstdlib>
//#include <stdbool.h>

#include "Nicla_System.h"
#include <Arduino_BHY2.h>

#define hight 44 // hight difference for 2 storeys


SensorActivity activity(SENSOR_ID_AR);
Sensor baro(SENSOR_ID_BARO);

// Global variables:
enum motion {Undifined = -1, Still, NotStill, Moving} movement = Undifined;

int storey = 2;   // starting floor = 0
int storey_baro;   // pressure at current floor
int counter = 0; 
int last_baro[8];
int avg_baro;   // to return last calculated avg
int last_avg;   // last avg_baro
int current_move = 0;
int last_move = 0;  // last moving direction
bool vert_movement; // actual vertical movement



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
  
  storey_baro = avg_baro = last_avg = baro_cal(10, 20);  // (#discarded, #base_average)
  
    
  //Serial.println("Activity:");
}

bool is_moving() {
  return movement == 2;
}


int AD() {  // activity detection
  switch(activity.value()) {
    case 256: if(movement != Still) {
      movement = Still;
      //Serial.println(movement); 
      }
      break;
    case 1: case 8226: case 36: 
      if(movement != NotStill) {
        movement = NotStill;
        //Serial.println(movement); 
      }
      break;
    case 512: case 1024: 
      if(movement != Moving) {
        movement = Moving;
        //Serial.println(movement); 
      }
      break;    
  }

  return movement;
}

int average(int array[], int start, int end) {
  int sum = 0;
  end++;  // index management, so that start/end are indices

  for(int i = start; i < end; i++) sum += array[i];   // sum
  
  return sum / (end - start);
}

void printarray(int array[]) {
  for(int i = 0; i < 8; i++) {
    Serial.print(String(array[i])+", ");
  }
  //Serial.print("\n");
}

int AB() {  //average baro
  switch(counter) {
      case 0: case 1: case 2: case 3: case 4: case 5: case 6: 
        last_baro[counter] = baro_value();
        counter++;
        break;
      
      case 7: last_baro[7] = baro_value();
        counter = 0;
        avg_baro = average(last_baro, 0, 7);  // average over whole array
        storey = FD();                        // refresh floor detection
        current_move = VM();                  // refresh current movement
        vert_movement = current_move && (current_move == last_move);  // securely detect vertical movement
        AD();                                 // refresh movement
        
        Serial.println("Stry: " + String(storey) + ",\tstairs: " + String(movement == 2 && vert_movement) + ",\tlift: " + String(movement != 2 && vert_movement));
        //printarray(last_baro);(

        last_move = current_move;
        last_avg = avg_baro;
        break;
  }
  
  return avg_baro;
}


int FD() {  // floor detection
  int difference = storey_baro - avg_baro;

  if(abs(difference) < (hight*3/4)) { // same storey
    return storey;
  }
  else if(difference > (hight*3/4) && difference < (hight*5/4)) { // storey up
    storey_baro -= hight;
    return ++storey;
  }
  else if(-difference > (hight*3/4) && difference < (hight*5/4)) { // storey down
    storey_baro += hight;
    return --storey;
  }
  return storey;  // fallback
}


int VM() {
  int score = 0;
  for(int i = 0; i < 7; i++) {
    if(last_baro[i] == last_baro[i+1]) {
      continue;
    }
    else if(last_baro[i] > last_baro[i+1]) {
      score--;
    }
    else score ++;    
  }

  if((avg_baro - last_avg) > 1 && score > -1) return 1;
  else if((avg_baro - last_avg) < -1 && score < 1) return -1;
  else return 0;
}


void loop() {
  static auto lastCheck = millis();

  if(millis()-lastCheck >= 100){
    lastCheck = millis();
    BHY2.update();

    

  AB();
  //Serial.println(String());
  
  }
}
