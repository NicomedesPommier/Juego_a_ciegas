#include <ArduinoBLE.h>



//Pins
#define pinX A0
#define pinY A1
#define motor 11
#define Buzzer 12
#define selected 10
#define POTENCIOMETRO A3
const int buttonPins[] = { 4, 5, 6, 7, 8, 9 };
//variables
int joyX, joyY;
bool sel;
int POT;


// Define the UUIDs for the characteristics
/* estos son como los nombre clave de los datos cuando la consola pide 
  datos le pide que le envie los datos que estan en este lugar 
  el otro codigo ya tiene los mismos
  */
// JOYSTICK
const char* RIGHTCharacteristicUUID = "3988c8a7-fda5-4a7a-8221-d7c46493f5e5";
const char* LEFTCharacteristicUUID = "f5d0f01c-6f85-401e-8f46-fd14569cbbd7";

//BUTTONS
const char* B1CharacteristicUUID = "0dacab49-2ed3-4f63-b795-421207aa2c42";
const char* B2CharacteristicUUID = "084ca3cb-3c15-4b51-b034-26cb38a2feb7";
const char* B3CharacteristicUUID = "b73ced0c-1e63-4d62-bdd5-0a5d9b3bd502";
const char* B4CharacteristicUUID = "8e31a810-1dc1-4330-83f3-d674d01e1aea";
const char* B5CharacteristicUUID = "3cc0487c-e43d-4c49-bf52-013b1f8fff71";
const char* B6CharacteristicUUID = "fc851925-501a-4a5f-be0e-7704be677155";
//POT
const char* POTCharacteristicUUID = "cd5b5a84-fe86-4681-bb56-5b21e80019aa";
// motor
const char* MotorCharacteristicUUID = "44e6f206-61e7-4d84-88cf-5df74f028bba";

//define the servie
/* el servicio es como el nombre principal cuando
  busca la conexion de bluetooth lo busca con estos numeros*/
BLEService joystickService("b04acf77-c173-4d63-a7bc-e17d4b52d324");

// define las characteristicas
/* aqui se define la funcion que tienen estos subgrupos
    BLERead es que la consola puede leer los datos
    BLENotify es que el control le avisa a la consola cuando cambia ese dato
    BLEWrite perimte que la consola escriba un valor en este subgrupo*/
// joystick char
BLEIntCharacteristic XCharacteristic(RIGHTCharacteristicUUID, BLERead | BLENotify);
BLEIntCharacteristic YCharacteristic(LEFTCharacteristicUUID, BLERead | BLENotify);

// buttons
BLEBoolCharacteristic B1Characteristic(B1CharacteristicUUID, BLERead | BLENotify);
BLEBoolCharacteristic B2Characteristic(B2CharacteristicUUID, BLERead | BLENotify);
BLEBoolCharacteristic B3Characteristic(B3CharacteristicUUID, BLERead | BLENotify);
BLEBoolCharacteristic B4Characteristic(B4CharacteristicUUID, BLERead | BLENotify);
BLEBoolCharacteristic B5Characteristic(B5CharacteristicUUID, BLERead | BLENotify);
BLEBoolCharacteristic B6Characteristic(B6CharacteristicUUID, BLERead | BLENotify);

//pot
BLEIntCharacteristic POTCharacteristic(POTCharacteristicUUID, BLERead | BLENotify);

//motor
BLEIntCharacteristic MotorCharacteristic(MotorCharacteristicUUID, BLERead | BLENotify | BLEWrite);

// time
long previousMillis = 0;  // last time the battery level was checked, in ms

void setup() {
  // set pins

  for (int i = 0; i < 6; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(Buzzer, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(selected, INPUT);
  // setear lo normal
  Serial.begin(9600);            // initialize serial communication
  pinMode(LED_BUILTIN, OUTPUT);  

  // Aqui comienza el seteo bluetoph no importa much
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1)
      ;
  }

  // set ble name
  BLE.setLocalName("JoyStick");
  BLE.setAdvertisedService(joystickService);  // se crea el servicio

  // se agregan la caracteristicas usadas antes al servicio creas estos subgrupos
  //Add the JOYSTICK characteristic
  joystickService.addCharacteristic(XCharacteristic);
  joystickService.addCharacteristic(YCharacteristic);

  // add button characteristic
  joystickService.addCharacteristic(B1Characteristic);
  joystickService.addCharacteristic(B2Characteristic);
  joystickService.addCharacteristic(B3Characteristic);
  joystickService.addCharacteristic(B4Characteristic);
  joystickService.addCharacteristic(B5Characteristic);
  joystickService.addCharacteristic(B6Characteristic);
  //add POT characteristic
  joystickService.addCharacteristic(POTCharacteristic);
  // Add the JOYSTICK service
  BLE.addService(joystickService);
  // set initial value for this characteristic
  XCharacteristic.writeValue(0);
  YCharacteristic.writeValue(0);


  /* Start advertising Bluetooth® Low Energy.  It will start continuously transmitting Bluetooth® Low Energy
     advertising packets and will be visible to remote Bluetooth® Low Energy central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth® device active, waiting for connections...");
}

void loop() {
  // wait for a Bluetooth® Low Energy central
    BLEDevice central = BLE.central();

  // SI la consola se conecta al control 
    if (central.connected()) {
      //RIGHTCharacteristic.broadcast();
      Serial.print("Connected to central: ");
      // print the central's BT address:
      Serial.println(central.address());
      // turn on the LED to indicate the connection:
      digitalWrite(LED_BUILTIN, HIGH);

    // mientras la consola este conectada va a enviar los daros cada 200ms 
      while (central.connected()) {
      tone(Buzzer,1000);
      delay(1000);
      noTone(Buzzer);
      delay(1000);

      long currentMillis = millis();
      // if 200ms have passed, check the JOYSTICK:
        if (currentMillis - previousMillis >= 200) {
          previousMillis = currentMillis;
          joyX = analogRead(pinX);
          joyY = analogRead(pinY);
          int ejeX = 0;
          int ejeY = 0;
        // aca no lo mapee pq no me daba valores consistentes esto funciono mejor
        // JOYSTICK VALUES
          if (400 <= joyX <= 600) {
          ejeX = 0;
          }
          if (joyX >= 601) {
          ejeX = 1;
          }
          if (joyX <= 399) {
            ejeX = -1;
          }
          if (400 <= joyY <= 600) {
            ejeY = 0;
          }
          if (joyY >= 601) {
            ejeY = 1;
          }
          if (joyY <= 399) {
            ejeY = -1;
          }
        //IMPORTANTE!!!!
        /* Aca cambia el valor de la caracteristica esta parte trabaja en 
        conjunto con la consola ya que si en el codigo de la consola no le digo 
        que lo lea este valor no cambia igual que leer un sensor*/
        /*las cosas estan definidas aca pq sino no las agarra 
        es mañoso nomas, pero asi funciona bien
        si algo tiene un nombre aca en el otro codigo puede tener otro nombre
        ejemplo B1 en la esp se usa para algo dentro de la misma placa por lo que no lo lee 
        por eso en el otro codigo dice BOT1
        writevalue(variable)escribe el dato de esa variable*/
        //valores joystick
          XCharacteristic.writeValue(ejeX);
          YCharacteristic.writeValue(ejeY);
          Serial.print("EJEX: ");
          Serial.println(ejeX);
          Serial.print("EJEY: ");
          Serial.println(ejeY);
        // BUTTONS VALUES
          bool B1 = digitalRead(buttonPins[0]);
          bool B2 = digitalRead(buttonPins[1]);
          bool B3 = digitalRead(buttonPins[2]);
          bool B4 = digitalRead(buttonPins[3]);
          bool B5 = digitalRead(buttonPins[4]);
          bool B6 = digitalRead(buttonPins[5]);
          B1Characteristic.writeValue(B1);
          B2Characteristic.writeValue(B2);
          B3Characteristic.writeValue(B3);
          B4Characteristic.writeValue(B4);
          B5Characteristic.writeValue(B5);
          B6Characteristic.writeValue(B6);
          Serial.print("B1: ");
          Serial.println(B1);
          Serial.print("B2: ");
          Serial.println(B2);
          Serial.print("B3: ");
          Serial.println(B3);
          Serial.print("B4: ");
          Serial.println(B4);
          Serial.print("B5: ");
          Serial.println(B5);
          Serial.print("B6: ");
          Serial.println(B6);
        // pot values
          POT = analogRead(POTENCIOMETRO);
          POTCharacteristic.writeValue(POT);
          Serial.print("POT: ");
          Serial.println(POT);
      }
    }

    // when the central disconnects, turn off the LED:
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}