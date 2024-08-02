#include <AccelStepper.h>

// Some variables to hold the numbers received over the serial monitor
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

int dataNumber = 0;             // new for this version


// This defines the pulse and direction pins for the stepper motor driver
#define pul0 2
#define dir0 3
#define MS0 4 
#define MS1 5
#define MS2 6
#define EN 7
 
// Define a stepper and the pins it will use
AccelStepper stepper(1,pul0, dir0); // Sets up stepper motor object

// Init motor position 
long pos = 0;
String inString = "";  // string to hold input


// Define some constants for converting steps to ml of fluid from the VCS
 

 
void setup()
{  
  // Setup pins for stepper motor driver config
  pinMode(MS0,OUTPUT);
  pinMode(MS1,OUTPUT);
  pinMode(MS2, OUTPUT);
  //  pinMode(EN,OUTPUT);
  digitalWrite(MS0,LOW);
  digitalWrite(MS1,HIGH);
  digitalWrite(MS2,LOW);
  // digitalWrite(EN,HIGH);


  // Setup Serial Com

  Serial.begin(9600);
  // Setup stepper motor run settings
  stepper.setMaxSpeed(1000); // Set max speed ofr stepper motor
  stepper.setAcceleration(1000);
  stepper.moveTo(1000);

}
 
void loop()
{
  // Define position here 
  if(stepper.distanceToGo()==0){
    stepper.moveTo(-stepper.currentPosition());
  }



// // Read in position from serial 
//   while (Serial.available() > 0) {
//     int inChar = Serial.read();
    
    
//     inString += (char)inChar;

//     // if (isDigit(inChar)) {
//     //   // convert the incoming byte to a char and add it to the string:
//     //   inString += (char)inChar;
//     // }
//     // if you get a newline, print the string, then the string's value:
//     if (inChar == '\n') {
//       pos = inString.toInt();
//       Serial.print("Value:");
//       Serial.println(pos);
 
//       // clear the string for new input:
//       inString = "";
//     }
//   }


//   // while (Serial.available() > 0) {
//   // pos = Serial.parseInt();
//   // }


    

// Move to position

  stepper.run();
  
//   stepper.moveTo(10);
//   stepper.setSpeed(750);
//   stepper.runSpeedToPosition();
}



