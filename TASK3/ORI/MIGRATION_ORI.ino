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

