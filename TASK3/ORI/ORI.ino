#include <Arduino_BHY2.h>
//#include "Nicla_System.h"

bool debugFlag = true;

#define MAXROTATIONS 200

SensorQuaternion rotationVector(SENSOR_ID_RV);
SensorXYZ magnetometer(SENSOR_ID_MAG);

int mag_x, mag_y, mag_z;
int lastMismatch;
float x, y, z, w;

enum magResult {MISALIGNED, MISMATCH, MATCH};

unsigned long timer = 0;
bool abnormalityDetected = false;
bool normalityDetected = false;
bool noValidPoint;
const int interval = 100;
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


// Places current orientation into array
void newOrientation() {
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




int mismatchScore() {

  static unsigned long lastUpdateTime = millis();

  if(isMoving()) {
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
      if (abnormalityDetected) {
        //Decrese score by one level
        if (score > 0 )
        {
          score -= 5;
        }
      } else
      if (normalityDetected) {
        if (score < 100)
        {
          score += 1;
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

int isOutside() {

}

bool isCalibrated(){
  if(orientationCount >= 99) {return true;}
}
bool isNewOrientation() {
  for(int i = 0; i < orientationCount; i++) {
    if(dotProduct(orientationArray[i]) >= 0.98) {
      return false;
    }
  }
  return true;
}

//DUMMY FUNCTION
bool isMoving()
{
  return true;
}

enum magResult matchesMagneticField() {
  for(int i = 0; i < orientationCount; i++) {
    //if(debugFlag){Serial.print( String( millis() ) );}
    if(dotProduct(orientationArray[i]) >= 0.99) {
      if(debugFlag){Serial.print("Orientation match, ");}
      int strength = calculateFieldStrength();
      if(strength <= orientationArray[i].fieldStrength + 100 && strength >= orientationArray[i].fieldStrength - 100){
          //Strength mismatch
          if(debugFlag){Serial.println("Strength match" + String(strength) + String(orientationArray[i].fieldStrength));}
          return MATCH;
      }
      // Strength mismatch
      lastMismatch = millis();
      if(debugFlag){Serial.println("Strength mismatch" + String(strength) + String(orientationArray[i].fieldStrength));}
      return MISMATCH;
    }
  }
  return MISALIGNED;
}

float dotProduct(Orientation vec2) {
  return currentOrientation.x * vec2.x + currentOrientation.y * vec2.y + currentOrientation.z * vec2.z + currentOrientation.w * vec2.w;
}

int calculateFieldStrength() {
  mag_x = magnetometer.x();
  mag_y = magnetometer.y();
  mag_z = magnetometer.z();

  return sqrt(mag_x*mag_x + mag_y*mag_y + mag_z*mag_z);
}


void orientationSetup() {
  BHY2.update();
  if(debugFlag){Serial.println("Initializing... Wait three seconds");}
  x = rotationVector.x();
  y = rotationVector.y();
  z = rotationVector.z();
  w = rotationVector.w();
  orientationArray[0] = {x,y,z,w, calculateFieldStrength()};
  previousOrientation = orientationArray[0];
}

void setup() {
  Serial.begin(115200);
  BHY2.begin();
  rotationVector.begin();
  magnetometer.begin();
  for(int i = 0; i<3000;i++)
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
  Serial.println("Mismatch score: " + String(mismatchScore()));
  //Serial.println(String(x) + ", " + String(y) + ", " + String(z) + ", " + String(w) + ", " + String(cosineSimilarityNormalized(previousOrientation)));
  previousOrientation = currentOrientation;
  delay(10);
}

