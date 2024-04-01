int Buzzer = 11
//LIBRERIAS
 // #include "SPI.h"
  //#include "Adafruit_GFX.h"
 // #include <Adafruit_SH110X.h>
  //#include "DFRobotDFPlayerMini.h"
  #include <ArduinoBLE.h>
// set audio
  //DFRobotDFPlayerMini myDFPlayer;

//height and width screen
  #define SCREEN_WIDTH 128  // OLED display width, in pixels
  #define SCREEN_HEIGHT 64  // OLED display height, in pixels
  #define OLED_RESET -1     // 
// set screen
  #define i2c_Address 0x3c
  //Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Safebox variables
  //posicion cuarto
    int room1_positionX;
    int room1_positionY;
  //inside room
    bool room1x = false;
    bool room1y = false;

//Braile | Morse Variables
  //posicion cuarto
    int room2_positionX;
    int room2_positionY;
  // flags
    bool miniGame2 = false; 
    bool MorseBraile = false;
  //inside Braile|Morse
    bool room2x = false;
    bool room2y = false;


//variables ble
    int ejeX = 0 ;
    int ejeY = 0 ;
    byte Bot1 = 0 ;
    byte Bot2 = 0;
    byte Bot3 =0;
    byte Bot4 =0;
    byte Bot5 =0;
    byte Bot6 =0;
    int POT =0;

// variable juego
  //tiempos
    int pausa = 100;
    int pausado = 300;
  //move variables
    int Xpos = 64;
    int Ypos = 32;

void setup() {
  Serial.begin(115200);

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  Serial.println("MF CONSOLE");

  // start scanning for peripherals
  BLE.scanForUuid("b04acf77-c173-4d63-a7bc-e17d4b52d324");
  
  //map setup
  room1_positionX = random(0, SCREEN_HEIGHT - 10);
  room2_positionX = random(0, SCREEN_HEIGHT - 10);

  room1_positionY = random(0, SCREEN_WIDTH - 10);
  room2_positionY = random(0, SCREEN_WIDTH - 10);

}

void loop() {
/* se setea el Bluetooth para poder conectar 
  chino mandarin*/
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
// UUID DEFINITION
  // joystick UUID
  BLECharacteristic XCharacteristicUUID      = peripheral.characteristic("3988c8a7-fda5-4a7a-8221-d7c46493f5e5");
  BLECharacteristic YCharacteristicUUID      = peripheral.characteristic("f5d0f01c-6f85-401e-8f46-fd14569cbbd7");
  //buttons UUID
  BLECharacteristic B1CharacteristicUUID     = peripheral.characteristic("0dacab49-2ed3-4f63-b795-421207aa2c42");
  BLECharacteristic B2CharacteristicUUID     = peripheral.characteristic("084ca3cb-3c15-4b51-b034-26cb38a2feb7");
  BLECharacteristic B3CharacteristicUUID     = peripheral.characteristic("b73ced0c-1e63-4d62-bdd5-0a5d9b3bd502");
  BLECharacteristic B4CharacteristicUUID     = peripheral.characteristic("8e31a810-1dc1-4330-83f3-d674d01e1aea");
  BLECharacteristic B5CharacteristicUUID     = peripheral.characteristic("3cc0487c-e43d-4c49-bf52-013b1f8fff71");
  BLECharacteristic B6CharacteristicUUID     = peripheral.characteristic("fc851925-501a-4a5f-be0e-7704be677155");
  //potenciometer UUID
  BLECharacteristic POTCharacteristicUUID    = peripheral.characteristic("cd5b5a84-fe86-4681-bb56-5b21e80019aa");
// check Characteristics
  /* esta parte de el codigo revisa si las caracteristicas necesarias
   estan en el servicio sumando 1 si la caracteristica necesaria esta*/
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
  // fin del chekeo
  /*si todas las caracteristicas estan se actualiza los datos del joystick cada 200ms 
  aca para aprovechar el while empieza todo lo del juego*/

  while (CHECK==9) {
// se leen los datos de el joystick
    XCharacteristicUUID.readValue(ejeX);
    YCharacteristicUUID.readValue(ejeY);
    B1CharacteristicUUID.readValue(Bot1);
    B2CharacteristicUUID.readValue(Bot2);
    B3CharacteristicUUID.readValue(Bot3);
    B4CharacteristicUUID.readValue(Bot4);
    B5CharacteristicUUID.readValue(Bot5);
    B6CharacteristicUUID.readValue(Bot6);
    POTCharacteristicUUID.readValue(POT);

  // datos 


   /* se imprimen los datos enviados por el joystick para debugear
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
    */

// comienza el loop de el juego
  MOVE();
  BraileMorse() ;
  }
// caso que se desconecte el mando
  // peripheral disconnected, start scanning again
   BLE.scanForUuid("b04acf77-c173-4d63-a7bc-e17d4b52d324");
  }
  }
// funcion para moverse en el mapa
  void MOVE(){

  //move x
  Serial.print("x: ");
  Serial.println(Xpos);
  if (ejeX == 1){
    Xpos += 1;
  }
  if (ejeX == -1){
    Xpos -= 1;
  }
  if (ejeX == 0){
    Xpos = Xpos;
  }
  if (Xpos <= 0){
    Xpos=0;
  }
   if (Xpos >= 128){
    Xpos=128;
  }
  //move Y
  Serial.print("y: ");
  Serial.println(Ypos); 
  if (ejeY == 1){
    Ypos += 1;
  }
  if (ejeY == -1){
    Ypos -= 1;
  }
  if (ejeY == 0){
    Ypos = Ypos;
  }
  if (Ypos <= 0){
    Ypos=0;
  }
   if (Ypos >= 128){
    Ypos=128;
  }


  }
// Funcion para ver si se esta dentro de algun cuarto


//ALPHABET Morse | Braile //

  void mymorse_A() {  //dot,dash
  //display.clearDisplay();
  //dot
  tone(Buzzer, 255);
  delay(100);
  //display.fillRect(40, 20, 10, 10, SH110X_WHITE);
  //pause
  noTone(Buzzer);
  delay(100);
  //dash
  tone(Buzzer, 255);
  delay(300);
  //display.fillRect(60, 20, 30, 10, SH110X_WHITE);
  //space
  //display.display();
  delay(1500);
  }
  void mymorse_B() {  //dash,dot,dot,dot
  // dash
  tone(Buzzer, 255);
  delay(300);
  //display.fillRect(20, 20, 30, 10, SH110X_WHITE);
  //pause
  delay(100);
  //dot 1
  tone(Buzzer, 255);
  delay(100);
  //display.fillRect(60, 20, 10, 10, SH110X_WHITE);
  //pause
  noTone(Buzzer);
  delay(100);
  //dot2
  tone(Buzzer, 255);
  delay(100);
  //display.fillRect(80, 20, 10, 10, SH110X_WHITE);
  //pause
  noTone(Buzzer);
  delay(100);
  //dot3
  tone(Buzzer, 255);
  delay(100);
  //display.fillRect(100, 20, 10, 10, SH110X_WHITE);
  //pause
  delay(1500);
  //display.display();
  } 
  void mymorse_K() {  //dash,dot,dash
  // dash
  tone(Buzzer, 255);
  delay(100);
  //display.fillRect(20, 20, 30, 10, SH110X_WHITE);
  //pause
  noTone(Buzzer);
  delay(100);
  //dot3
  tone(Buzzer, 255);
  delay(100);
  //display.fillRect(60, 20, 10, 10, SH110X_WHITE);
  //pause
  noTone(Buzzer);
  delay(100);
  // dash
  //display.fillRect(80, 20, 30, 10, SH110X_WHITE);
  tone(Buzzer, 255);
  delay(300);
  //display.display();
  noTone(Buzzer);
  delay(1500);  
  }

//Funcion de el Juego Braile | Morse 
  void BraileMorse(){
  //Variables para Morse | Braile
    int matchingIndex = -1;
    char detectedLetter;
    long t1, t2;


    bool ALPHABET[24][6] = {
    { 1, 0, 0, 0, 0, 0 },  //A
    { 1, 1, 0, 0, 0, 0 },  //B
    { 1, 0, 0, 1, 0, 0 },  //C
    { 1, 0, 0, 1, 1, 0 },  //D
    { 1, 0, 0, 0, 1, 0 },  //E
    { 1, 1, 0, 1, 0, 0 },  //F
    { 1, 1, 0, 1, 1, 0 },  //G
    { 1, 1, 0, 0, 1, 0 },  //H
    { 0, 1, 0, 1, 0, 0 },  //I
    { 0, 1, 0, 1, 1, 0 },  //J
    { 1, 0, 1, 0, 0, 0 },  //K
    { 1, 0, 1, 1, 0, 0 },  //L
    { 1, 0, 1, 0, 1, 0 },  //M
    { 1, 1, 1, 1, 0, 0 },  //N
    { 1, 1, 1, 1, 1, 0 },  //O
    { 1, 1, 1, 0, 1, 0 },  //P
    { 0, 1, 1, 1, 0, 0 },  //Q
    { 0, 1, 1, 1, 1, 0 },  //R
    { 1, 0, 1, 0, 0, 1 },  //S
    { 1, 1, 1, 0, 0, 1 },  //T
    { 0, 1, 0, 1, 1, 1 },  //U
    { 1, 0, 1, 1, 0, 1 },  //V
    { 1, 0, 1, 1, 1, 1 },  //W
    { 1, 0, 1, 0, 1, 1 }   //X
   };
    char letters[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

  //letras a usar para el juego A B k
    int R1 = 0;
    int R2 = 1;
    int R3 = 10;
    int R4 = 3;
  // respuestas
    bool Ans1 = false;
    bool Ans2 = false;
    bool Ans3 = false;
    bool Ans4 = false;
    bool Morse = false;

  //Morse | Braile mecaninca de juego
    while (miniGame2 == true) {
      if (Ans1 == false) {
        mymorse_A();
      }
      if (Ans2 == false && Ans1 == true) {
        mymorse_B();
     }
       if (Ans3 == false && Ans2 == true) {
        mymorse_K();
      }

      bool BUTTONS[]= {Bot1, Bot2, Bot3, Bot4, Bot5, Bot6} ;
      for (int i = 0; i < 24; i++) {
        bool match = true;
        for (int j = 0; j < 6; j++) {

          bool input_B = BUTTONS[j];
          //Serial.println(input_B);
          //Serial.print("         ");
          //Serial.print(j);

          if (input_B != ALPHABET[i][j]) {
            match = false;
            //Serial.println(buttonPins[j]);
            break;
          }
        }
        if (match) {
          int matchingIndex = i;
          Serial.println(matchingIndex);
          detectedLetter = letters[matchingIndex];
          Serial.println(detectedLetter);
          if (matchingIndex == R1 && Ans1 == false) {
            Serial.println("godines");
            /*display.setCursor(0, 0);
            display.print(detectedLetter);
            display.display();*/
            //myDFPlayer.play(9);  //GOOD
            delay(500);
            Ans1 = true;

            tone(Buzzer, 1600, 100);
            delay(500);
          }
          if (matchingIndex == R2 && Ans1 == true && Ans2 == false) {
            Serial.println("godines");
            /*display.setCursor(0, 0);
            display.print(detectedLetter);
            display.display();*/
           // myDFPlayer.play(9);  //GOOD
            delay(500);
            Ans2 = true;
            delay(500);
          }
          if (matchingIndex == R3 && Ans2 == true && Ans3 == false) {
            Serial.println("godines");
            /*display.setCursor(0, 0);
            display.print(detectedLetter);
            display.display();*/
           // myDFPlayer.play(9);  //GOOD
            delay(500);
            Ans3 = true;
            delay(500);
          }
          if (Ans3 == true && Morse == false) {
            Morse = true;
            miniGame2 = false;
            MorseBraile = true;
            Serial.println("Un exito");
            /*display.clearDisplay();
            display.setCursor(64, 32);
            display.print("well done");
            display.display();*/
            //myDFPlayer.play(7);  //GREAT
            delay(1500);
          }
          break;
        }
      }
      delay(100);
    }
  }