// Wire Receiver #3, address 10


#include <Wire.h>

String inputString = "";

void setup() {
  Wire.begin(10);                // join I2C bus with address #9
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()

void receiveEvent(int howMany) {

  //convert string back to float and print
  for(int i = 0; i < howMany; i++)
  {
    char c = Wire.read(); // receive byte as a character
    inputString += c;
  }
  float num = inputString.toFloat();
  inputString = "";
  Serial.println(num);
}
