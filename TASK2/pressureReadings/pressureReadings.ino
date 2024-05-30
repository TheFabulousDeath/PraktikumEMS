#include "Arduino_BHY2.h"
#include "Nicla_System.h"

#define DELAY 60000
#define REPS 50

Sensor baro(SENSOR_ID_BARO);

double pressure;
long milliseconds;
int counter = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nicla::begin();
  BHY2.begin();
  baro.begin();
  Serial.println("Time/ms,Pressure/hPa");
}

void loop() {
  // put your main code here, to run repeatedly:
  static auto lastCheck= millis();
  BHY2.update();
  if(counter <= REPS){
    if ((milliseconds = millis()) - lastCheck >= DELAY) {
      lastCheck = milliseconds;
      pressure = baro.value();
      Serial.println(String(milliseconds) + "," + String(pressure));
      counter++;
    }
  } else {delay(1000);}
}
