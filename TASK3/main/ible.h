/*

This head file contains the framework to send the BLE messages.

TODO implement following functions
	notifyState();
	notifyData();
	
*/


#ifndef ible_h
#define ible_h

#include "Arduino_BHY2.h"
#include "Nicla_System.h"
#include <ArduinoBLE.h>


// Erzeugt UUID
#define BLE_SENSE_UUID(val) ("19a10031-" val "-537e-4f6c-d104768a1214")



const int VERSION = 0x00000000;

BLEService service(BLE_SENSE_UUID("0000"));

// Datenfeld was übertragen wird
BLEUnsignedIntCharacteristic versionCharacteristic(BLE_SENSE_UUID("1001"), BLERead);
BLECharacteristic magCharacteristic(BLE_SENSE_UUID("1101"), BLERead | BLENotify, 21 + sizeof(int)); // String (20 chars + null terminator) and int

String stateString;
String name;

SensorXYZ mag(SENSOR_ID_MAG);

void BLEsetup() {
  
  BLE.begin();

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


void sendState(int* floor) {
 	if(BLE.connected() && magCharacteristic.subscribed()) {
		
      String data = "MagData"; // Example string, replace with actual data as needed
      char buffer[25] = {0}; // Initialize the buffer to zero

      // Copy string to buffer and ensure it's null-terminated
      stateString.toCharArray(buffer, 21); // This ensures the string is null-terminated and fits within 20 characters

      // Copy integer to buffer
      memcpy(buffer + 21, floor, sizeof(int));

      // Write value to characteristic
      magCharacteristic.writeValue((unsigned char*)buffer, sizeof(buffer));
    }
}


#endif