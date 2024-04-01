#include <ArduinoBLE.h>

// variables for button
bool up, down, left, right = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  Serial.println("MF CONSOLE");

  // start scanning for peripherals
  BLE.scanForUuid("b04acf77-c173-4d63-a7bc-e17d4b52d324");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();



    // stop scanning
    BLE.stopScan();

    // connect to the peripheral
    Serial.println("Connecting ...");

    if (peripheral.connect()) {
      Serial.println("Connected");
    }   
    else {
      Serial.println("Failed to connect!");
      return;
    }
    // discover peripheral attributes
    Serial.println("Discovering attributes ...");
    if (peripheral.discoverAttributes()) {
      Serial.println("Attributes discovered");
    }
     else {
      Serial.println("Attribute discovery failed!");
      peripheral.disconnect();
      return;
    }
  int characteristicCount = peripheral.characteristicCount();
  int ServiceCount = peripheral.serviceCount();
  Serial.print(ServiceCount);
  Serial.println(" Service discovered");
  Serial.print(characteristicCount);
  Serial.println(" characteristis discovered");
// joystick
BLECharacteristic RIGHTCharacteristicUUID  = peripheral.characteristic("3988c8a7-fda5-4a7a-8221-d7c46493f5e5");
BLECharacteristic LEFTCharacteristicUUID   = peripheral.characteristic("f5d0f01c-6f85-401e-8f46-fd14569cbbd7");
BLECharacteristic UPCharacteristicUUID     = peripheral.characteristic("d62eee79-1e61-4576-b50f-1b245c70956d");
BLECharacteristic DOWNCharacteristicUUID   = peripheral.characteristic("7575c6ae-20e8-4c68-867c-ad01c234684a");
//buttons
BLECharacteristic B1CharacteristicUUID     = peripheral.characteristic("0dacab49-2ed3-4f63-b795-421207aa2c42");
BLECharacteristic B2CharacteristicUUID     = peripheral.characteristic("084ca3cb-3c15-4b51-b034-26cb38a2feb7");
BLECharacteristic B3CharacteristicUUID     = peripheral.characteristic("b73ced0c-1e63-4d62-bdd5-0a5d9b3bd502");
BLECharacteristic B4CharacteristicUUID     = peripheral.characteristic("8e31a810-1dc1-4330-83f3-d674d01e1aea");
BLECharacteristic B5CharacteristicUUID     = peripheral.characteristic("3cc0487c-e43d-4c49-bf52-013b1f8fff71");
BLECharacteristic B6CharacteristicUUID     = peripheral.characteristic("fc851925-501a-4a5f-be0e-7704be677155");
//potenciometer
BLECharacteristic POTCharacteristicUUID    = peripheral.characteristic("cd5b5a84-fe86-4681-bb56-5b21e80019aa");
// check
  if (RIGHTCharacteristicUUID==1){
  Serial.println(" has R Char");
    }
  if (LEFTCharacteristicUUID==1){
  Serial.println(" has l Char");
    }
  if (UPCharacteristicUUID==1){
  Serial.println(" has U Char");
    }
  if (DOWNCharacteristicUUID==1){
  Serial.println(" has D Char");
    }
  if (B1CharacteristicUUID==1){
  Serial.println(" has B1 Char");
    }
  if (B2CharacteristicUUID==1){
  Serial.println(" has B2 Char");
    }

   if (B3CharacteristicUUID==1){
  Serial.println(" has B3 Char");
    }
   if (B4CharacteristicUUID==1){
  Serial.println(" has B4 Char");
    }
  if (B5CharacteristicUUID==1){
  Serial.println(" has B5 Char");
    }
  if (B6CharacteristicUUID==1){
  Serial.println(" has B6 Char");
    }  
  if (POTCharacteristicUUID==1){
  Serial.println(" has POT Char");
}    
  while (RIGHTCharacteristicUUID==1) {
    byte value = 0 ;
    RIGHTCharacteristicUUID.readValue(value);
    Serial.println(value);


    /*
    if (RIGHTCharacteristicUUID.valueUpdated()) {
      // set to 1
      right = 1;
      left = 0;
      delay(200);
      Serial.println(right);

    }
    else{
      right = 0;
      left  = 0;
      Serial.println(right);
    }*/
    }

    

    // peripheral disconnected, start scanning again
   // BLE.scanForUuid("b04acf77-c173-4d63-a7bc-e17d4b52d324");
  }
}




