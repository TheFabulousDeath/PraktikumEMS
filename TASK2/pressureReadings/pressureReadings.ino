#include "Arduino_BHY2.h"
#include "Nicla_System.h"

SensorXYZ baro(SENSOR_ID_BARO);

int baroX, baroY, baroZ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nicla::begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  baroX = baro.x();
  baroY = baro.y();
  baroZ = baro.z();

  Serial.println(String(baroX) + "," + String(baroX) + "," + String(baroX));
}
