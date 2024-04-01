//LIBRERIAS
 // #include "SPI.h"
  #include "Adafruit_GFX.h"
  #include <Adafruit_SH110X.h>
  //#include "DFRobotDFPlayerMini.h"
  #include <ArduinoBLE.h>
// set audio
  //DFRobotDFPlayerMini myDFPlayer;

//height and width screen
  #define SCREEN_WIDTH 128  // OLED display width, in pixels
  #define SCREEN_HEIGHT 64  // OLED display height, in pixels
  #define OLED_RESET -1     // 
// set screen
  #define i2c_Address 0x3C
  Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Safebox variables
  bool n1_value;
  bool n2_value;
  bool n3_value;
  int n1;
  int n2;
  int n3;
  //posicion cuarto
    int room1_positionX;
    int room1_positionY;
  //inside room
    bool InSafeBoxR = 0;



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
 // flags juego
     bool inSafeBoxX;
     bool inBraileX;
     bool inSafeBoxY;
     bool inBraileY;
     bool SB;
     bool MB;
     //VICTORIA
     bool G1;
     bool G2;
// variables pantalla (todo mapa visual)

//Braile | Morse Variables
  //posicion cuarto
    int room2_positionX;
    int room2_positionY;  
  // flags 
    bool MorseBraile = false;
  //inside Braile|Morse
    bool InMorse = 0;
    //Morse | Braile
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
  bool win1;
  bool win2;



void setup() {
  Serial.begin(115200);

  // initialize the Bluetooth® Low Energy hardware
    BLE.begin();

    Serial.println("MF CONSOLE");

    // start scanning for peripherals
    BLE.scanForUuid("b04acf77-c173-4d63-a7bc-e17d4b52d324");
  
  //map setup
    room1_positionX = random(0, SCREEN_WIDTH - 10);
    room2_positionX = random(0, SCREEN_WIDTH - 10);
    
    room1_positionY = random(0, SCREEN_HEIGHT - 10);
    room2_positionY = random(0, SCREEN_HEIGHT - 10);
    //print room positions for knowing
      Serial.print("Safebox position: x, y ");
      Serial.print(room1_positionX);
      Serial.print("  ");
      Serial.println(room1_positionY);
      Serial.println("Morse position: x, y ");
      Serial.print(room2_positionX);
      Serial.print("  ");
      Serial.println(room2_positionY);
  //Screen Setup
    delay(250); // wait for the OLED to power up
    display.begin(i2c_Address, true); // Address 0x3C default

 
    display.display();
    delay(2000);

    // Clear the buffer.
    display.clearDisplay();

    // draw a single pixel
    display.drawPixel(10, 10, SH110X_WHITE);
    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands
    // to make them visible on the display hardware!
    display.display();
    delay(2000);
    display.clearDisplay();
  //set SB variables
    n1 = random(0, 100);
    n2 = random(0, 100);
    n3 = random(0, 100);


  delay(1000);
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
      display.setCursor(50 , 20);
      display.setTextColor(SH110X_WHITE);
      display.setTextSize(0.2);
      display.println("Please reset Console");
      display.display();
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
  BLECharacteristic motorCharacteristicUUID    = peripheral.characteristic("44e6f206-61e7-4d84-88cf-5df74f028bba");
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
  map();
  MOVE();
  dentro();
  // morse Braile
  while (MB == 1 ){
    display.clearDisplay();
    display.display();
    B1CharacteristicUUID.readValue(Bot1);
    B2CharacteristicUUID.readValue(Bot2);
    B3CharacteristicUUID.readValue(Bot3);
    B4CharacteristicUUID.readValue(Bot4);
    B5CharacteristicUUID.readValue(Bot5);
    B6CharacteristicUUID.readValue(Bot6);

   bool input_B[]={Bot1, Bot2, Bot3,Bot4,Bot5,Bot6};
   if (Ans1 == false) {

      
      //mymorse_A();
    }
    if (Ans2 == false && Ans1 == true) {
      //mymorse_B();
    }
    if (Ans3 == false && Ans2 == true) {
      //mymorse_K();
    }

    for (int i = 0; i < 24; i++) {
      bool match = true;
      for (int j = 0; j < 6; j++) {
        bool input_Braile = input_B[i];
        Serial.println(input_Braile);
        Serial.print("         ");
        Serial.print(j);

        if (input_B[i] != ALPHABET[i][j]) {
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
          display.setCursor(0, 0);
          display.print(detectedLetter);
          display.display();
          //myDFPlayer.play(9);  //GOOD
          motorCharacteristicUUID.writeValue(1);
          delay(500);
          Ans1 = true;

          //tone(Buzzer, 1600, 100);
          delay(500);
        }
        if (matchingIndex == R2 && Ans1 == true && Ans2 == false) {
          Serial.println("godines");
          display.setCursor(0, 0);
          display.print(detectedLetter);
          display.display();
          //myDFPlayer.play(9);  //GOOD
          delay(500);
          Ans2 = true;
          delay(500);
        }
        if (matchingIndex == R3 && Ans2 == true && Ans3 == false) {
          Serial.println("godines");
          display.setCursor(0, 0);
          display.print(detectedLetter);
          display.display();
          //myDFPlayer.play(9);  //GOOD
          delay(500);
          Ans3 = true;
          delay(500);
        }

        if (Ans3 == true && Morse == false) {
          Morse = true;
          MorseBraile = true;
          MB = 0 ;
          Serial.println("Un exito");
          display.clearDisplay();
          display.print("well done");
          display.display();
          G2=true;
         // myDFPlayer.play(7);  //GREAT
          delay(1500);
        }
        break;
      }
    }
    delay(100);


  }
  // SAFEBOX game
    while(SB==1){
    const int centerX = 32;
    const int centerY = 64;
    const int radius = 20;
    // set screen to mini game view
    POTCharacteristicUUID.readValue(POT);
    display.clearDisplay();
    display.drawCircle(64, 32, radius, SH110X_WHITE);
    //leer datos
    int previousReading;
    int PotMap = map(POT, 5, 1022, 0, 100);
    int rads = map(PotMap, 0, 100, 0, 360);
    // cambiar datos
    float angleRad = radians(PotMap);
    int lineEndX = centerX + (radius * sin(angleRad));
    int lineEndY = centerY - (radius * cos(angleRad));
    // dibujar linea del candado
    display.drawLine(centerY, centerX, lineEndY, lineEndX, SH110X_WHITE);
    Serial.println(n1_value);
    Serial.println(n2_value);
    Serial.println(n3_value);
    Serial.println(n1);
    Serial.println(n2);
    Serial.println(n3);
    Serial.println(PotMap);
    display.display();
    //safebox  mechanics
    if (PotMap % 2 == 0 && PotMap != previousReading) {
      previousReading = PotMap;
      //tone(Buzzer, tik, 100);
      //audio.play(2);  //Play the tik mp3
    }
    if (PotMap == n1 && n1_value == false) {  //n1 value is reached
      n1_value = true;
      //tone(Buzzer, tok, 100);
      //myDFPlayer.play(9);  //GOOD
      delay(500);
      // audio.play(1);  //Play the tok mp3
    }
    if (n1_value == true && PotMap == n2 && n2_value == false) {  //n2 value is reached
      n2_value = true;
      //tone(Buzzer, tok, 100);
      //myDFPlayer.play(9);  //GOOD
      delay(500);
      //audio.play(1);  //Play the tok mp3
    }
    if (n2_value == true && PotMap == n3 ) {  //n3 value is reached
      n3_value = true;
      //tone(Buzzer, tok, 100);
      //myDFPlayer.play(9);  //GOOD
      delay(500);
      //audio.play(1);  //Play the tok mp3
    }
    if (n3_value == true) {
      display.clearDisplay();
      display.display();
      display.setCursor(32, 32);
      display.setTextSize(1);
      display.println("Well Done!!!!");
      display.display();
      //myDFPlayer.play(7);  //GOOD
      delay(1500);
      G1 = 1;

  

    } 
  if (G1==1 && G2==1){
    display.clearDisplay();
    display.setCursor(30, 34);
    display.println("ESCAPASTE!");
  }
  
  }
  }
// caso que se desconecte el mando
  // peripheral disconnected, start scanning again
   BLE.scanForUuid("b04acf77-c173-4d63-a7bc-e17d4b52d324");
  }
  }
// funcion para moverse en el mapa
  void MOVE(){

  //move x
  Serial.print(" "); 
  Serial.print("x: ");
  Serial.println(Xpos);
  if (ejeX == 1){
    Xpos += 5;
  }
  if (ejeX == -1){
    Xpos -= 5;
  }
  if (ejeX == 0){
    Xpos = Xpos;
  }
//AQUI VAN SONIDOS
  if (Xpos <= 0){
    Xpos=1;
  }
   if (Xpos >= 120){
    Xpos=119;
  }
//
  //move Y
  if (ejeY == 1){
    Ypos += 5;
  }
  if (ejeY == -1){
    Ypos -= 5;
  }
  if (ejeY == 0){
    Ypos = Ypos;
  }
  if (Ypos <= 0){
//AQUI VAN SONIDOS
    Ypos=1;
  }
   if (Ypos >= 60){
    Ypos=59;
  }
  Serial.print("y: ");
  Serial.println(Ypos); 


  }
//
// Funcion para ver si se esta dentro de algun cuarto(faltan sonidos) 
    void dentro (){

    // revisa si esta detntro de el eje X los cuartos
    //DENTRO
    if (room1_positionX - 5 <= Xpos && Xpos<= room1_positionX + 5){
      inSafeBoxX = 1;
    }
    //FUERA
    else{
      inSafeBoxX = 0;
    }
    //DENTRO
    if (room2_positionX - 5 <= Xpos && Xpos<= room2_positionX + 5){
      inBraileX = 1;
    }
    //FUERA
    else{
      inBraileX = 0;
    }
    //REVISA SI ESTA DENTRO DE EL CUARTO EN EL EJE Y
    //DENTRO
    if (room1_positionY - 5 <= Ypos && Ypos  <= room1_positionY + 5){
      inSafeBoxY = 1;
    }
    //FUERA
    else{
      inSafeBoxY = 0;
    }
    //DENTRO
    if (room2_positionY - 5 <= Ypos && Ypos <= room2_positionY + 5){
      inBraileY = 1;
    }
    //FUERA
    else{
      inBraileY = 0;
    }
    // dentro de safebox
    if(inSafeBoxX==1 && inSafeBoxY==1 ){
      InSafeBoxR = true;
      Serial.print("SafeBox Room");
    }
    else{
      InSafeBoxR = false;
    }
    if(inBraileX==1 && inBraileY==1 ){
      InMorse = true;
      Serial.print("Morse | Braile Room");
    }
    else{
      InMorse = false;
    }
    if(InSafeBoxR == true){
      display.clearDisplay();
      display.setCursor( 20,64 );
      display.setTextColor(SH110X_WHITE);
      display.setTextSize(0.5);
      display.println("SafeBox Comienza");
      SB = 1;
      
      

    }
    if(InMorse == true ){
      display.clearDisplay();
      display.setCursor( 20,64 );
      display.setTextColor(SH110X_WHITE);
      display.setTextSize(0.5);
      display.println("MORSE Comienza");
      MB = 1;
    
      

    }


  }
// funcion mapa visual pantalla
  void map(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SH110X_WHITE);
  //SAFEBOX
  display.drawRect(room1_positionX, room1_positionY, 10, 10, SH110X_WHITE);
  display.setCursor(room1_positionX - 10, room1_positionY);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(0.2);
  display.println("SBox");

  //Morse|braile
  display.drawRect(room2_positionX, room2_positionY, 10, 10, SH110X_WHITE);
  display.setCursor(room2_positionX - 10, room2_positionY);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(0.2);
  display.println("M | B");
  // main Door
  display.drawRect(10, 0, 5, 5, SH110X_WHITE);
  //JUGADOR
  display.fillRect(Xpos, Ypos, 5, 5, SH110X_WHITE);
  display.display();
  delay(200);

  }


//ALPHABET//
void mymorse_A() {  //dot,dash
  display.clearDisplay();
  //dot
  
  display.fillRect(40, 20, 10, 10, SH110X_WHITE);
  //pause
  //dash
  
  display.fillRect(60, 20, 30, 10, SH110X_WHITE);
  //space
  display.display();
  delay(1500);
}
void mymorse_B() {  //dash,dot,dot,dot
  // dash
  display.fillRect(20, 20, 30, 10, SH110X_WHITE);
  //pause
  //dot 1

  display.fillRect(60, 20, 10, 10, SH110X_WHITE);
  //pause
  
  //dot2

  display.fillRect(80, 20, 10, 10, SH110X_WHITE);
  //pause
  //dot3
  display.fillRect(100, 20, 10, 10, SH110X_WHITE);
  //pause
  delay(1500);
  display.display();
}
void mymorse_K() {  //dash,dot,dash
  // dash
  
  display.fillRect(20, 20, 30, 10, SH110X_WHITE);
  //pause
  //dot3

  display.fillRect(60, 20, 10, 10, SH110X_WHITE);
  //pause
  // dash
  display.fillRect(80, 20, 30, 10, SH110X_WHITE);
  display.display();
  delay(1500);
}

