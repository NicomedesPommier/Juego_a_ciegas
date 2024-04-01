#include <SoftwareSerial.h>

// Define the RX and TX pin numbers
const int rxPin = 2;  // Connect to the TX pin of the other device
const int txPin = 3;  // Connect to the RX pin of the other device

// Create a SoftwareSerial instance
SoftwareSerial mySerial(rxPin, txPin);

void setup() {
  // Start the serial communication
  Serial.begin(9600);  // Used for debugging, connected to the computer's serial port
  mySerial.begin(9600);  // Used for communication with the other device
}

void loop() {
  // Read data from the SoftwareSerial port and print it on the Serial Monitor
  while (mySerial.available()) {
    char data = mySerial.read();
    Serial.print(data);
  }
  
  // Send data from the Serial Monitor to the SoftwareSerial port
  while (Serial.available()) {
    char data = Serial.read();
    mySerial.print(data);
  }
}
