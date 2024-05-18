#include "Arduino_BHY2.h"
#include "Nicla_System.h"

#define DELAY 60000


Sensor baro(SENSOR_ID_BARO);

double pressure;
long milliseconds;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nicla::begin();
  BHY2.begin();
  baro.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  static auto lastCheck= millis();
  BHY2.update();

  if ((milliseconds = millis()) - lastCheck >= DELAY) {
    lastCheck = milliseconds;
    pressure = baro.value();
    Serial.println(String(milliseconds) + "," + baro.toString());
  }
}
