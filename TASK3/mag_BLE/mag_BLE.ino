#include "Arduino_BHY2.h"
#include "Nicla_System.h"
#include <ArduinoBLE.h>


// Erzeugt UUID
#define BLE_SENSE_UUID(val) ("19b10000-" val "-537e-4f6c-d104768a1214")

const int VERSION = 0x00000000;

BLEService service(BLE_SENSE_UUID("0000"));

// Datenfeld was übertragen wird
BLEUnsignedIntCharacteristic versionCharacteristic(BLE_SENSE_UUID("1001"), BLERead);
BLECharacteristic magCharacteristic(BLE_SENSE_UUID("1101"), BLERead | BLENotify, 3 * sizeof(float)); // Array of 3x 2 Bytes, XY

String name;

SensorXYZ mag(SENSOR_ID_MAG_BIAS);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  nicla::begin();
  BHY2.begin();
  mag.begin();

  if (!BLE.begin()){
    Serial.println("Failed to initialized BLE!");

    while (1)
      ;
  }


  //Don't touch for now
  String address = BLE.address();

  Serial.print("address = ");
  Serial.println(address);

  address.toUpperCase();

  name = "NiclaSenseME-";
  name += address[address.length() - 5];
  name += address[address.length() - 4];
  name += address[address.length() - 2];
  name += address[address.length() - 1];

  Serial.print("name = ");
  Serial.println(name);

  BLE.setLocalName(name.c_str());
  BLE.setDeviceName(name.c_str());
  BLE.setAdvertisedService(service);

  //Add Characteristics (siehe Zeile 12)
  service.addCharacteristic(magCharacteristic);
  service.addCharacteristic(versionCharacteristic);
  //No sensor event handlers
  

  versionCharacteristic.setValue(VERSION);
  //Advertise
  BLE.addService(service);
  BLE.advertise();
}


void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  while (BLE.connected()){
    BHY2.update();

    if(magCharacteristic.subscribed())
    {
      float x,y,z;

      x = mag.x();
      y = mag.y();
      z = mag.z();

      float magValues[3] = {x, y, z};
      Serial.println(String((int) sqrt(x*x + y*y + z*z)));
      magCharacteristic.writeValue(magValues, sizeof(magValues));
    }
  }
}
