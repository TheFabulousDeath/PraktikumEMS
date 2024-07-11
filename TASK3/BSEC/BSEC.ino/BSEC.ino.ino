#include "Arduino_BHY2.h"
#include "Nicla_System.h"

SensorBSEC bsec(SENSOR_ID_BSEC);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nicla::begin();
  BHY2.begin();
  bsec.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  BHY2.update();
  delay(1000);
  Serial.println( bsec.toString() );
}
