#include <Arduino_BHY2.h>
//#include "Nicla_System.h"

bool debugFlag = true;

SensorQuaternion rotationVector(SENSOR_ID_RV);
SensorXYZ magnetometer(SENSOR_ID_MAG);

float x, y, z, w;
int mag_x, mag_y, mag_z;

class Orientation {
public:
  float x;
  float y;
  float z;
  float w;
};

class MagneticFieldEntry {
public:
  int entries;
  int averageStrength;
  Orientation orientation;
};

Orientation orientationArray[100];
int orientationCount = 1;
Orientation currentOrientation;
Orientation previousOrientation;

void newOrientation() {
  if(isNewOrientation()) {
    if (orientationCount < 100) {
      orientationArray[orientationCount] = currentOrientation;
      orientationCount++;
      if(debugFlag) {
        Serial.println(String(millis()) + ", New Orientation found. Count: " + String(orientationCount));
      }
    }
  }
}

bool isNewOrientation() {
  for(int i = 0; i < orientationCount; i++) {
    if(dotProduct(orientationArray[i]) >= 0.90) {
      return false;
    }
  }
  delay(100);
  return true;
}

float dotProduct(Orientation vec2) {
  return currentOrientation.x * vec2.x + currentOrientation.y * vec2.y + currentOrientation.z * vec2.z + currentOrientation.w * vec2.w;
}

void orientationSetup() {
  BHY2.update();
  x = rotationVector.x();
  y = rotationVector.y();
  z = rotationVector.z();
  w = rotationVector.w();
  orientationArray[0] = {x,y,z,w};
  previousOrientation = {x,y,z,w};
}
/*#include <Arduino_BHY2.h>
//#include "Nicla_System.h"
#include <vector>

bool debugFlag = true;

SensorQuaternion rotationVector(SENSOR_ID_RV);
SensorXYZ magnetometer(SENSOR_ID_MAG);

float x, y, z, w;
int mag_x, mag_y, mag_z;

class Orientation {
public:
  float x;
  float y;
  float z;
  float w;
};

class magneticFieldEntry {
  int entries;
  int averageStrength;
  Orientation orientation;
};

std::vector<Orientation> orientationVector = {};
Orientation currentOrientation;
Orientation previousOrientation;


void newOrientation() {
  if(isNewOrientation()){
    orientationVector.push_back(currentOrientation);
    if(debugFlag){Serial.println(String(millis()) + ", New Orientation found. Length: " + String(orientationVector.size()));}
  }
}

bool isNewOrientation() {
  for(auto i : orientationVector) 
  {
    if(dotProduct(i)>=0.95) {return false;}
  }
  delay(100);
  return true;
}

float dotProduct(Orientation vec2) {
    return currentOrientation.x * vec2.x + currentOrientation.y * vec2.y + currentOrientation.z * vec2.z + currentOrientation.w * vec2.w;
}
*/
/*
float magnitude(Orientation vec) {
    float sum = vec.x*vec.x + vec.y*vec.y + vec.z*vec.z + vec.w*vec.w;
    return sqrt(sum);
}
*/

/* 
THIS FUNCTION ASSUMES VECTORS WITH A TWO-NORM > 1, QUATERNIONS HAVE A NORM OF 1 AT ALL TIMES
float cosineSimilarity(Orientation vec1, Orientation vec2) {
    float dotProd = dotProduct(vec1, vec2);
    float mag1 = magnitude(vec1);
    float mag2 = magnitude(vec2);

    if (mag1 == 0 || mag2 == 0) {
        // Handle the case where one vector is zero vector, resulting in undefined cosine similarity
        return 0.0;
    }

    return dotProd  / (mag1 * mag2);
}
*/

float cosineSimilarityPercent(Orientation vec2) {
    float dotProd = dotProduct(vec2);
    return 100*dotProd;
}

void setup() {
  Serial.begin(115200);
  BHY2.begin();
  rotationVector.begin();
  
  orientationSetup();
  Serial.println("SETUP COMPLETE");
}

void loop() {
  BHY2.update();
  x = rotationVector.x();
  y = rotationVector.y();
  z = rotationVector.z();
  w = rotationVector.w();
  
  currentOrientation = {x,y,z,w};
  newOrientation();
  //Serial.println(String(x) + ", " + String(y) + ", " + String(z) + ", " + String(w) + ", " + String(cosineSimilarityNormalized(previousOrientation)));
  previousOrientation = currentOrientation;
}

