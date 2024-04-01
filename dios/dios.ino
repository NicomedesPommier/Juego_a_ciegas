#include <ArduinoBLE.h>

// BLE Service and Characteristic UUIDs
#define SERVICE_UUID "19B10010-E8F2-537E-4F6C-D104768A1214"
#define CHARACTERISTIC_UUID "19B10011-E8F2-537E-4F6C-D104768A1214"

BLEService bleService(SERVICE_UUID); // Create the BLE service
BLECharacteristic bleCharacteristic(CHARACTERISTIC_UUID, BLERead | BLENotify); // Create the BLE characteristic

void setup() {
  Serial.begin(9600);
  
  // Initialize the BLE module
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE!");
    while (1);
  }

  // Set the local name peripheral advertises
  BLE.setLocalName("MyPeripheral");
  BLE.setAdvertisedService(bleService); // Add the service UUID
  BLE.addService(bleService); // Add the service
  bleCharacteristic.setValue("Hello, Central!"); // Set initial characteristic value

  // Start advertising
  BLE.advertise();

  Serial.println("Peripheral device advertising...");
}

void loop() {
  // Check if a central device is connected
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      // Read and print the value of the characteristic
      String value = bleCharacteristic.value();
      Serial.print("Characteristic value: ");
      Serial.println(value);

      delay(2000); // Wait for 2 seconds
      
      // Update the characteristic value
      bleCharacteristic.setValue("Updated value!");

      // Notify the central device about the updated value
      bleCharacteristic.notify();
    }

    Serial.println("Central disconnected");
  }
}
