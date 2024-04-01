//librerias
#include "SPI.h"
#include "Adafruit_GFX.h"
#include <Adafruit_SH110X.h>
#include <SD.h>  // need to include the SD library
#include "SoftwareSerial.h" // que buscar el cambio de esto
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer;

//define pins
#define motor 12
#define Buzzer 11
#define sel 10
const int buttonPins[] = { 4, 5, 6, 7, 8, 9 };
#define knob A1
#define horz A3
#define vert A2
//pines pantalla no se definen ni nada es asi
//SCK A5 y SDA A4

//height and width screen
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET -1     //   QT-PY / XIAO
int W = 128;
int H = 64;
// set screen
#define i2c_Address 0x3c
Adafruit_SH1106G display = Adafruit_SH1106G(W, H, &Wire, OLED_RESET);

// general game setups
//mini game flags
bool miniGame1 = false;
bool miniGame2 = false;
bool miniGame3 = false;
//inside room
bool room1x = false;
bool room1y = false;
//inside room
bool room2x = false;
bool room2y = false;
bool roomx = false;
bool roomy = false;

//set joystick
int joyx;
int joyy;
bool selPressed;
int vertical = 0;
int horizontal = 0;
int roomsize = 100;

// dMovment
int xDir = 20;
int yDir = 20;
bool insidemapx;
bool insidemapy;
bool moved;

//safe box variables
int n1;         //1st number
int n2;         //2nd number
int n3;         //3rd number
int knob_data;  //Potenciometer data for game
int gameknob;
int hold = 500;
long t_1;
long t_2;
int previousReading = 0;
int PotMap = 0;
boolean n1_value = false;  //true if value is reached
boolean n2_value = false;  //true if value is reached
boolean n3_value = false;  //true if value is reached
int centerX = 32;
int centerY = 64;
int radius = 50;
int tik = 440;
int tok = 261;

// map variables
int room1_positionX;
int room2_positionX;
int room3_positionX;
int room1_positionY;
int room2_positionY;
int room3_positionY;
int limitex = W;
int limitey = H;
//EndGame Variables
boolean safeBox = false;
boolean MorseBraile = false;
boolean game3 = true;
boolean Key = false;
long gameTime;
// initial position of the Player

int xPos = centerX;
int yPos = centerY;
// variables to keep track of the Player location
int xPrev = xPos;
int yPrev = yPos;

// set up the mp3 playr
SoftwareSerial mySoftwareSerial(2,3); // RX, TX
DFRobotDFPlayerMini audio;
bool ACT = false;

//Morse | Braile
int matchingIndex = -1;
char detectedLetter;
long t1, t2;
const int pause = 100;

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

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
void printDetail(uint8_t type, int value);

// funciones
void move() {
  // revisar si esta en limites
  if (xPos >= 0 && xPos <= limitex) {
    insidemapx = true;
  }
  if (yPos >= 0 && yPos <= limitex) {
    insidemapy = true;
  }


  if (insidemapx == true && insidemapy == true) {
    if (joyx >= 600) {
      yPos = yPos - xDir;
      moved = true;
    }
    if (joyx <= 200) {
      yPos = yPos + xDir;
      moved = true;
    }
    if (joyy >= 600) {
      xPos = xPos - xDir;
      moved = true;
    }
    if (joyy <= 200) {
      xPos = xPos + xDir;
      moved = true;
    }
    if (moved) {
      //audio.play(1);  //Play the first mp3 move mp3
      Radar();
      moved = false;
    }

  } else {
    Serial.print("outside ");
  }
  Serial.print("x: ");
  Serial.println(xPos);
  Serial.print("y: ");
  Serial.println(yPos);
  display.clearDisplay();
  display.display();

  delay(200);
}
void mapa() {
  display.setCursor(0, 0);
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SH110X_WHITE);
  display.drawRect(room1_positionY, room1_positionX, 10, 10, SH110X_WHITE);
  display.setCursor(room1_positionY - 10, room1_positionX);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(0.2);
  display.println("SBox");

  //Morse|braile
  display.drawRect(room2_positionY, room2_positionX, 10, 10, SH110X_WHITE);
  display.setCursor(room2_positionY - 10, room2_positionX);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(0.2);
  display.println("M | B");
  // main Door
  display.drawRect(10, 0, 5, 5, SH110X_WHITE);

  display.fillRect(yPos, xPos, 5, 5, SH110X_WHITE);

  display.display();
}
void safeBoxRoom() {
  if (xPos >= room1_positionX && xPos <= room1_positionX + roomsize) {
    room1x = true;
  }

  if (yPos >= room1_positionY && yPos <= room1_positionY + roomsize) {
    room1y = true;
  }

  else {
    room1x = false;
    room1y = false;
  }
  if (room1y == true && room1x == true && safeBox == false) {
    Serial.println("Safe Box");
    tone(Buzzer, 3600);
    delay(200);
    //audio.play(1);  //Play the inside room mp3

    if (selPressed == false) {
      noTone(Buzzer);
      ACT = true;
      myDFPlayer.play(6);  //SELECT
      delay(500);
      display.clearDisplay();
      display.setCursor(centerX, centerY);
      display.print("Safe Box mini game");
      display.display();

      miniGame1 = true;
      room1x = false;
      room1y = false;
      if (ACT == true) {
        myDFPlayer.play(3);  //Audio de candado
        delay(24000);
        ACT = false;
      }
      delay(2000);
      //Audio Inicio juego CANDADO
      //audio.play(1);  //Play the mini game mp3
    }
  }
}
void SafeBox() {
  while (miniGame1 == true) {
    // set screen to mini game view
    display.clearDisplay();
    display.display();
    display.drawCircle(64, 32, radius, SH110X_WHITE);
    display.display();
    //leer datos
    selPressed = digitalRead(sel);
    knob_data = analogRead(knob);
    PotMap = map(knob_data, 5, 1022, 0, 100);
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
    if (knob_data % 2 == 0 && PotMap != previousReading) {
      previousReading = PotMap;
      tone(Buzzer, tik, 100);
      //audio.play(2);  //Play the tik mp3
    }
    if (PotMap == n1 && n1_value == false) {  //n1 value is reached
      n1_value = true;
      tone(Buzzer, tok, 100);
      myDFPlayer.play(9);  //GOOD
      delay(500);
      // audio.play(1);  //Play the tok mp3
    }
    if (n1_value == true && PotMap == n2 && n2_value == false) {  //n2 value is reached
      n2_value = true;
      tone(Buzzer, tok, 100);
      myDFPlayer.play(9);  //GOOD
      delay(500);
      //audio.play(1);  //Play the tok mp3
    }
    if (n2_value == true && PotMap == n3 && knob_data == n3) {  //n3 value is reached
      n3_value = true;
      tone(Buzzer, tok, 100);
      myDFPlayer.play(9);  //GOOD
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
      safeBox = true;
      miniGame1 = false;
      myDFPlayer.play(7);  //GOOD
      delay(1500);

      tone(Buzzer, 2600);
      digitalWrite(motor, HIGH);
      delay(500);
      tone(Buzzer, 3600);
      digitalWrite(motor, LOW);
      delay(500);
      tone(Buzzer, 2600);
      digitalWrite(motor, HIGH);
      delay(500);
      tone(Buzzer, 3600);
      digitalWrite(motor, LOW);
      delay(500);
      tone(Buzzer, 2600);
      digitalWrite(motor, HIGH);
      delay(500);
      tone(Buzzer, 3600);
      digitalWrite(motor, LOW);
      delay(1000);
      display.clearDisplay();
      display.display();
      //audio.play(1);  //Play the well done mp3
      break;
    }
    if (selPressed == false) {
      display.clearDisplay();
      display.setCursor(centerX, centerY);
      display.println("Saliste del mini juego");
      display.display();
      miniGame1 = false;
      myDFPlayer.play(6);  //Play the first mp3
      delay(2000);
      break;
    }
  }
}
void BraileRoom() {
  if (xPos >= room2_positionX && xPos <= room2_positionX + roomsize) {
    room2x = true;
  }
  if (yPos >= room2_positionY && yPos <= room2_positionY + roomsize) {
    room2y = true;
  } else {
    room2x = false;
    room2y = false;
  }
  if (room2y == true && room2x == true && Morse == false) {
    Serial.println("Morse | Braile");
    room2x = false;
    room2y = false;
    //audio.play(1);  //Play the inside mp3
    if (selPressed == false) {
      noTone(Buzzer);
      ACT = true;
      myDFPlayer.play(6);  //S
      delay(500);
      display.clearDisplay();
      display.setCursor(centerX, centerY);
      display.print("Morse | braile minigame");
      display.display();
      miniGame2 = true;
      // audio.play(1);  //Pulay the out mp3
      if (ACT == true) {
        myDFPlayer.play(2);  //Audio de braile
        delay(25000);
        ACT = false;
      }
    }
  }
}
void MorseBraileGame() {
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

    for (int i = 0; i < 24; i++) {
      bool match = true;
      for (int j = 0; j < 6; j++) {
        bool input_B = digitalRead(buttonPins[j]);
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
          display.setCursor(0, 0);
          display.print(detectedLetter);
          display.display();
          myDFPlayer.play(9);  //GOOD
          delay(500);
          Ans1 = true;

          tone(Buzzer, 1600, 100);
          delay(500);
        }
        if (matchingIndex == R2 && Ans1 == true && Ans2 == false) {
          Serial.println("godines");
          display.setCursor(0, 0);
          display.print(detectedLetter);
          display.display();
          myDFPlayer.play(9);  //GOOD
          delay(500);
          Ans2 = true;
          delay(500);
        }
        if (matchingIndex == R3 && Ans2 == true && Ans3 == false) {
          Serial.println("godines");
          display.setCursor(0, 0);
          display.print(detectedLetter);
          display.display();
          myDFPlayer.play(9);  //GOOD
          delay(500);
          Ans3 = true;
          delay(500);
        }
        /*if(matchingIndex == 3 && Ans3 == true && Ans4 == false){
            Serial.println("godines");
            Ans4= true;
            delay(500);
          }       */
        if (Ans3 == true && Morse == false) {
          Morse = true;
          miniGame2 = false;
          MorseBraile = true;
          Serial.println("Un exito");
          display.clearDisplay();
          display.setCursor(centerX, centerY);
          display.print("well done");
          display.display();
          myDFPlayer.play(7);  //GREAT
          delay(1500);
        }
        break;
      }
    }
    delay(100);
  }
}
void escape() {
  if (safeBox == true && MorseBraile == true && game3 == true) {
    Key = true;
    myDFPlayer.play(8);  //WIN
    delay(2500);
  }
  //audio.play(1);  //Play the all mini games won mp3
}
void EndGame() {
  if (Key == true) {
    if (xPos >= 0 && xPos <= 40) {
      roomx = true;
    }
    if (yPos >= 0 && yPos <= 40) {
      roomy = true;
    } else {
      roomx = false;
      roomy = false;
    }
    if (roomy == true && roomx == true) {
      Serial.println("Inside");
      if (selPressed == false) {
        roomx = false;
        roomy = false;
        //audio.play(1);  //Play the won song mp3
        gameTime = millis() / (60000);
        myDFPlayer.play(10);  //GOOD
        delay(25000);
        display.clearDisplay();
        display.setCursor(20, centerY);
        display.println("You Escaped in :");
        display.print("gameTime");
        display.print("minutes");
        display.display();
        Serial.println(" You Escaped in ");
        Serial.print(gameTime);
        Serial.println(" minutes");
        delay(5000);
      }
    }
  }
}
void Radar() {
  bool fary;
  bool farx;
  bool closex;
  bool closey;
  bool boundsx;
  bool boundsy;
  bool inbounds;
  // out of limits
  if (xPos >= W || xPos <= 0) {
    tone(Buzzer, 200);
    inbounds = false;
    delay(100);
  }
  if (yPos >= H || yPos <= 0) {
    tone(Buzzer, 100);
    inbounds = false;
    delay(100);
  }
  if (xPos <= W || xPos >= 0) {
    boundsx = true;
  }
  if (xPos <= H || xPos >= 0) {
    boundsy = true;
  }
  if (boundsy && boundsx) {
    inbounds = true;
  }

  if (safeBox == false && inbounds == true) {
    if (xPos >= (room1_positionX + roomsize + 50) || xPos <= (room1_positionX - roomsize - 50)) {
      farx = true;
    }
    if (yPos >= (room1_positionY + roomsize + 50) || yPos <= (room1_positionY - roomsize - 50)) {
      fary = true;
    }
    //Close sound
    if (xPos <= (room1_positionX + roomsize + 50) && xPos >= (room1_positionX - roomsize - 50)) {
      closex = true;
    }
    if (yPos <= (room1_positionY + roomsize + 50) && yPos >= (room1_positionY - roomsize - 50)) {
      closey = true;
    }
  }
  if (Morse == false && inbounds == true) {
    if (xPos >= (room2_positionX + roomsize + 50) || xPos <= (room2_positionX - roomsize - 50)) {
      farx = true;
    }
    if (yPos >= (room2_positionY + roomsize + 50) || yPos <= (room2_positionY - roomsize - 50)) {
      fary = true;
    }
    //Close sound
    if (xPos <= (room2_positionX + roomsize + 50) && xPos >= (room2_positionX - roomsize - 50)) {
      closex = true;
    }
    if (yPos <= (room2_positionY + roomsize + 50) && yPos >= (room2_positionY - roomsize - 50)) {
      closey = true;
    }
  }
  //sounds_joystick
  if (farx == true && farx == false) {
    tone(Buzzer, 523, pause);
  }
  if (fary == true && farx == false) {
    tone(Buzzer, 587, pause);
  }
  if (farx == true && fary == true) {
    tone(Buzzer, 659, pause);
  }
  if (closex == true && closey == false) {
    tone(Buzzer, 1000, pause);
  }

  if (closey == true && closex == false) {
    tone(Buzzer, 784, pause);
  }
  if (closey == true && closex == true) {
    tone(Buzzer, 880, pause);
  }
}
//ALPHABET//
void mymorse_A() {  //dot,dash
  display.clearDisplay();
  //dot
  tone(Buzzer, 255, pause);
  display.fillRect(40, 20, 10, 10, SH110X_WHITE);
  //pause
  delay(pause);
  //dash
  tone(Buzzer, 255, pause * 3);
  display.fillRect(60, 20, 30, 10, SH110X_WHITE);
  //space
  display.display();
  delay(1500);
}
void mymorse_B() {  //dash,dot,dot,dot
  // dash
  tone(Buzzer, 255, pause * 3);
  display.fillRect(20, 20, 30, 10, SH110X_WHITE);
  //pause
  delay(pause);
  //dot 1
  tone(Buzzer, 255, pause);
  display.fillRect(60, 20, 10, 10, SH110X_WHITE);
  //pause
  delay(pause);
  //dot2
  tone(Buzzer, 255, pause);
  display.fillRect(80, 20, 10, 10, SH110X_WHITE);
  //pause
  delay(pause);
  //dot3
  tone(Buzzer, 255, pause);
  display.fillRect(100, 20, 10, 10, SH110X_WHITE);
  //pause
  delay(1500);
  display.display();
}
void mymorse_K() {  //dash,dot,dash
  // dash
  tone(Buzzer, 255, pause * 3);
  display.fillRect(20, 20, 30, 10, SH110X_WHITE);
  //pause
  delay(pause);
  //dot3
  tone(Buzzer, 255, pause);
  display.fillRect(60, 20, 10, 10, SH110X_WHITE);
  //pause
  delay(pause);
  // dash
  display.fillRect(80, 20, 30, 10, SH110X_WHITE);
  tone(Buzzer, 255, pause * 3);
  display.display();
  delay(1500);
}


void setup() {
  // pins setups
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(Buzzer, OUTPUT);
  pinMode(motor, OUTPUT);
  noTone(Buzzer);
  pinMode(sel, INPUT);


//mp3 Settings
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
      ;
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(10);  //WIN
  delay(2500);
  myDFPlayer.play(1);  //Audio de bienvenida
  delay(34000);

  // safebox setup
  n1 = random(0, 100);  //1st number
  n2 = random(0, 100);  //2nd number
  n3 = random(0, 100);  //3rd number
  //map setup
  room1_positionX = random(0, H - 10);
  room2_positionX = random(0, H - 10);
  room3_positionX = random(0, H - 10);
  room1_positionY = random(0, W - 10);
  room2_positionY = random(0, W - 10);
  room3_positionY = random(0, W - 10);

  //screen setup
  delay(250);                        // wait for the OLED to power up
  display.begin(i2c_Address, true);  // Address 0x3C default
  display.display();
  delay(200);


  // Clear the buffer.
  display.clearDisplay();

  // draw a single pixel
  display.drawPixel(10, 10, SH110X_WHITE);
  // Show the display buffer on the hardware.
  // NOTE: You must call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  delay(2000);
  display.clearDisplay();
  mapa();
  display.display();    // hace que la wea pase
}
void loop() {
  static unsigned long timer = millis();

  /*if (millis() - timer > 3000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every 3 second.
  }*/

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());  //Print the detail message from DFPlayer to handle different errors and states.
  }

  joyx = analogRead(vert);
  joyy = analogRead(horz);
  selPressed = digitalRead(sel);
  Serial.println(selPressed);
  noTone(Buzzer);

  mapa();
  move();
  safeBoxRoom();
  BraileRoom();
  MorseBraileGame();
  //Game3Room();
  SafeBox();
  escape();
  EndGame();
}
