#include <ArduinoBLE.h>

// variables for button


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
BLECharacteristic XCharacteristicUUID      = peripheral.characteristic("3988c8a7-fda5-4a7a-8221-d7c46493f5e5");
BLECharacteristic YCharacteristicUUID      = peripheral.characteristic("f5d0f01c-6f85-401e-8f46-fd14569cbbd7");
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
  int CHECK = 0;
  if (XCharacteristicUUID==1){
    Serial.println(" has X Char");
    CHECK = CHECK +1;
    }
  if (YCharacteristicUUID==1){
    Serial.println(" has Y Char");
    CHECK = CHECK +1;
    }
  if (B1CharacteristicUUID==1){
    Serial.println(" has B1 Char");
    CHECK = CHECK +1;
    }
  if (B2CharacteristicUUID==1){
    Serial.println(" has B2 Char");
    CHECK = CHECK +1;
    }

  if (B3CharacteristicUUID==1){
    Serial.println(" has B3 Char");
    CHECK = CHECK +1;
    }
  if (B4CharacteristicUUID==1){
    Serial.println(" has B4 Char");
    CHECK = CHECK +1;
    }
  if (B5CharacteristicUUID==1){
    Serial.println(" has B5 Char");
    CHECK = CHECK +1;
    }
  if (B6CharacteristicUUID==1){
    Serial.println(" has B6 Char");
    CHECK = CHECK +1;
    }  
  if (POTCharacteristicUUID==1){
    Serial.println(" has POT Char");
    CHECK = CHECK +1;
    }    
  while (CHECK==9) {
    int ejeX = 0 ;
    int ejeY = 0 ;
    byte Bot1 = 0 ;
    byte Bot2 = 0;
    byte Bot3 =0;
    byte Bot4 =0;
    byte Bot5 =0;
    byte Bot6 =0;
    int POT =0;
    XCharacteristicUUID.readValue(ejeX);
    YCharacteristicUUID.readValue(ejeY);
    B1CharacteristicUUID.readValue(Bot1);
    B2CharacteristicUUID.readValue(Bot2);
    B3CharacteristicUUID.readValue(Bot3);
    B4CharacteristicUUID.readValue(Bot4);
    B5CharacteristicUUID.readValue(Bot5);
    B6CharacteristicUUID.readValue(Bot6);
    POTCharacteristicUUID.readValue(POT);
    Serial.print("ejeX: ");
    Serial.println(ejeX);
    Serial.print("ejeY: ");
    Serial.println(ejeY);
    Serial.print("B1: ");
    Serial.println(Bot1);
    Serial.print("B2: ");
    Serial.println(Bot2);
    Serial.print("B3: ");
    Serial.println(Bot3);
    Serial.print("B4: ");
    Serial.println(Bot4);
    Serial.print("B5: ");
    Serial.println(Bot5);
    Serial.print("B6: ");
    Serial.println(Bot6);
    Serial.print("POT: ");
    Serial.println(POT);

  }
    // peripheral disconnected, start scanning again
   BLE.scanForUuid("b04acf77-c173-4d63-a7bc-e17d4b52d324");
  }
}