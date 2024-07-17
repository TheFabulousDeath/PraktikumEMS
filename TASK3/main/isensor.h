#ifndef isensor_h
#define isensor_h

#include "Arduino_BHY2.h"
#include "Nicla_System.h"

#define MAXROTATIONS 150

SensorQuaternion rotationVector(SENSOR_ID_RV);
SensorXYZ magnetometer(SENSOR_ID_MAG);


class ISensor {
public:
	bool isCalibrated();
	bool hasLevelChange();
	int isOutside();

private:
	
};

class Activity : public ISensor {

  public:
    static bool is_moving()
    {
      return true;
    }
	
};

class Magnetometer : public ISensor {
public:
	bool isCalibrated(){
	  if(orientationCount >= MAXROTATIONS) {return true;}
    return false;
	}
	
	void setup() {
    
		//REMOVE SERIAL
	
	  Serial.begin(115200);
	  BHY2.begin();
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

	  if(Activity::is_moving()) 
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
	bool debugFlag = true;	
	
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

class Thermometer : public ISensor {
	
};

class AQI : public ISensor {
	
};

class Barometer : public ISensor {
	
};
#endif
