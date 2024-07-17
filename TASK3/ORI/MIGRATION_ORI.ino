#include <Arduino_BHY2.h>
//#include "Nicla_System.h"













// Places current orientation into array







int isOutside() {

}



//DUMMY FUNCTION
bool is_moving()
{
  return true;
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

