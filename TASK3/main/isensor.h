#ifndef isensor_h
#define isensor_h

#include "Arduino_BHY2.h"
#include "Nicla_System.h"

#include <cstdio>
#include <cstdlib>

#define MAXROTATIONS 100 // Array length.

#define hight 44 // hight difference for 2 storeys

SensorQuaternion rotationVector(SENSOR_ID_RV);
SensorXYZ magnetometer(SENSOR_ID_MAG);
SensorActivity activity(SENSOR_ID_AR);
Sensor baro(SENSOR_ID_BARO);

bool movingFlag = false;

class ISensor {

  
public:
	bool isCalibrated();
	bool hasLevelChange();
	int isOutside();


private:
	
};

class Activity : public ISensor {


  public:
  
  bool isCalibrated() {
    return true;
  }
  
	bool is_moving() {
	  movingFlag = (movement == 2);
	  return movingFlag;
	}
	
	void setup() {
	  activity.begin();
	  baro.begin();
	  storey_baro = avg_baro = last_avg = baro_cal(10, 20);  // (#discarded, #base_average)
	}


	void loop() {
	  static auto lastCheck = millis();

	  if(millis()-lastCheck >= 100){
		lastCheck = millis();
		BHY2.update();
	  AB();
    }
	}
  
  
  private:

	enum motion {Undifined = -1, Still, NotStill, Moving} movement = Undifined;

	int storey = 0;   // starting floor = 0
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
	
	int average(int array[], int start, int end) {
	  int sum = 0;
	  end++;  // index management, so that start/end are indices

	  for(int i = start; i < end; i++) sum += array[i];   // sum
	  
	  return sum / (end - start);
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
}; // END class Activity : public ISensor

class Magnetometer : public ISensor {


public:


	bool isCalibrated(){
	  if(orientationCount >= MAXROTATIONS) {return true;}
    return false;
	}
	
	void setup() {
    
		//REMOVE SERIAL
	

	  rotationVector.begin();
	  magnetometer.begin();
	  for(int i = 0; i<1000;i++)
	  {
	    BHY2.update();
	    delay(1);
	    Serial.println(String(i) + "/3000");
	  }
	  orientationSetup();
	  Serial.println("SETUP COMPLETE");
	}

  void loop() {
  BHY2.update();
	  x = rotationVector.x();
	  y = rotationVector.y();
	  z = rotationVector.z();
	  w = rotationVector.w();
	  
	  currentOrientation = {x,y,z,w,calculateFieldStrength()};
	  newOrientation();
	  matchesMagneticField();
	  if(debugFlag && isCalibrated()) {Serial.println("Is outside: " + String(isOutside()));}
	  previousOrientation = currentOrientation;
	  delay(10);
  }

  int isOutside() {
	  static unsigned long lastUpdateTime = millis();

	  if(movingFlag)
	  {
	    timer += millis() - lastUpdateTime;
	    lastUpdateTime = millis();

	    if(matchesMagneticField() == MISMATCH) {
	      abnormalityDetected = true;
	    }
	    if(matchesMagneticField() == MATCH){
	      normalityDetected = true;
	    }

	    // Check if 5 seconds have passed
	    if(timer >= interval){
	      if (normalityDetected) {
		//Decrese score by one level
		if (score < 100 )
		{
		  score += 5;
		}
	      } else
	      if (abnormalityDetected) {
		if (score > 0)
		{
		  score -= 5;
		}
	      }

	      // Reset timer and flag
	      timer = 0;
	      abnormalityDetected = false;
	      normalityDetected = false;
	    }
	  } else {
	    // Reset the last update time to pause the timer
	    lastUpdateTime = millis();
	  }

	  return score;
	}
	
	
private:


	bool debugFlag = false;	
	
	int mag_x, mag_y, mag_z;
	int lastMismatch;
	float x, y, z, w;
	enum magResult {MISALIGNED, MISMATCH, MATCH};
	
	unsigned long timer = 0;
	bool abnormalityDetected = false;
	bool normalityDetected = false;
	bool noValidPoint;
	const int interval = 3000;
	int score = 0;

	class Orientation {
	public:
	  float x;
	  float y;
	  float z;
	  float w;
	  int fieldStrength;
	};

	Orientation orientationArray[MAXROTATIONS];
	int orientationCount = 1;
	Orientation currentOrientation;
	Orientation previousOrientation;
	
	void newOrientation() 
	{
	  if(isNewOrientation()) {
	    if (orientationCount < MAXROTATIONS) {
	      orientationArray[orientationCount] = currentOrientation;
	      orientationCount++;
	      if(debugFlag) {
		Serial.println(String(millis()) + ", New Orientation found. Count: " + String(orientationCount));
	      }
	    }
	  }
	}
		
	int calculateFieldStrength() {
	  mag_x = magnetometer.x();
	  mag_y = magnetometer.y();
	  mag_z = magnetometer.z();

	  return sqrt(mag_x*mag_x + mag_y*mag_y + mag_z*mag_z);
	}
	
	enum magResult matchesMagneticField() 
	{
	  for(int i = 0; i < orientationCount; i++) {
	    //if(debugFlag){Serial.print( String( millis() ) );}
	    if(dotProduct(orientationArray[i]) >= 0.99) {
	      if(debugFlag && isCalibrated()){Serial.print("Orientation match, ");}
	      int strength = calculateFieldStrength();
	      if(strength <= orientationArray[i].fieldStrength + 100 && strength >= orientationArray[i].fieldStrength - 100){
		  //Strength mismatch
		  if(debugFlag && isCalibrated()){Serial.println("Strength match" + String(strength) + String(orientationArray[i].fieldStrength));}
		  return MATCH;
	      }
	      // Strength mismatch
	      lastMismatch = millis();
	      if(debugFlag && isCalibrated()){Serial.println("Strength mismatch" + String(strength) + String(orientationArray[i].fieldStrength));}
	      return MISMATCH;
	    }
	  }
	  return MISALIGNED;
	}
	
	float dotProduct(Orientation vec2) {
	  return currentOrientation.x * vec2.x + currentOrientation.y * vec2.y + currentOrientation.z * vec2.z + currentOrientation.w * vec2.w;
	}

	void orientationSetup() 
	{
	  BHY2.update();
	  if(debugFlag){Serial.println("Initializing... Wait three seconds");}
	  x = rotationVector.x();
	  y = rotationVector.y();
	  z = rotationVector.z();
	  w = rotationVector.w();
	  orientationArray[0] = {x,y,z,w, calculateFieldStrength()};
	  previousOrientation = orientationArray[0];
	}
	
	bool isNewOrientation() {
	  for(int i = 0; i < orientationCount; i++) {
	    if(dotProduct(orientationArray[i]) >= 0.98) {
	      return false;
	    }
	  }
	  return true;
	}
};

class AQI : public ISensor {
	
};
#endif
