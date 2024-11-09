//Declare pin functions on Arduino

// #define dir 2 // IMAN CHANGED THESE PINS FROM THE ORIGINAL EESA PINS 
// #define stp 3


// #define MS3 5
// #define MS2 6
// #define MS1 7
// #define EN  8


#define dir 3 // IF USING BREAD BOARD UNCOMMENT THIS AND COMMENT ABOVE PIN DEFINITIONS!!!! OTHERWISE COMMENT THIS AND USE ABOVE DEFINITIONS
#define stp 2


#define MS3 6
#define MS2 5
#define MS1 4
#define EN  7

//Declare variables for functions
char user_input;
int x;
int y;
int state;
int steps = 3075.03;
int position = 0;
float constant = steps/50;
int volume = 0;
int maximum = 200;
//990 steps = 50cc volume

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  Serial.println("Begin motor control");
  Serial.println();
  //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. Fill syringe certain amount");
  Serial.println("2. Empty syringe certain amount");
  Serial.println("3. Empty syringe completely");
  Serial.println("4. Fill syringe completely (200 CC)");
  Serial.println();
}


//Main loop
void loop() {
  
  while(Serial.available()){
    Serial.println("got here");
      user_input = Serial.read(); //Read user input and trigger appropriate function
      digitalWrite(EN, LOW); //Pull enable pin low to set FETs active and allow motor control
      if (user_input =='1')
      {
        SemiFillSyringe();
      }
      else if(user_input =='2')
      {
        SemiEmptySyringe();
      }
      else if(user_input =='3')
      {
        EmptySyringe();
      }
      else if(user_input =='4')
      {
        FillSyringe();
      }
      else
      {
        Serial.println("Invalid option entered.");
      }
      resetBEDPins();
  }
}


//Function to let user choose how much volume they would like entered into the system.
void SemiFillSyringe()
{
  Serial.println("Enter how much volume (CCs) you would like to enter system: ");

  while(Serial.available() == 0) {}
  volume = Serial.parseInt();
  position = position + volume*constant;

  if (position > maximum*constant){
    Serial.println("There is only " + String((maximum*constant - position + volume*constant)/constant) + " CC left to fill, please enter a value less than that.");
    position = position - volume*constant;
  }

  else{
    digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
    for(x= 0; x<volume*constant; x++)  //Loop the forward stepping till the desired volume is filled
    {
      digitalWrite(stp,HIGH); //Trigger one step forward
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(2);
    }
  }
  
  //Reprint out menu
  Serial.println();
  Serial.println("Enter number for control option:");
  Serial.println("1. Fill syringe certain amount");
  Serial.println("2. Empty syringe certain amount");
  Serial.println("3. Empty syringe completely");
  Serial.println("4. Fill syringe completely (200 CC)");
  Serial.println();

}

//Function to let user choose how much volume they would like leaving the system.
void SemiEmptySyringe()
{
  Serial.println("Enter how much volume (CCs) you would like to leave system: ");

  while(Serial.available() == 0) {}
  volume = Serial.parseInt();
  Serial.println(volume*constant);

  position = position - volume*constant;

  if (position < 0){
    Serial.println("There is only " + String((position+volume*constant)/constant) + " CC left to empty, please enter a value less than that.");
    position = position + volume*constant;
  }

  else{
    digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
    for(x= 0; x<volume*constant; x++)  //Loop the backward stepping till the desired volume is emptied.
    {
      digitalWrite(stp,HIGH); //Trigger one step
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(2);
    }
  }

  //Reprint out menu
  Serial.println();
  Serial.println("Enter number for control option:");
  Serial.println("1. Fill syringe certain amount");
  Serial.println("2. Empty syringe certain amount");
  Serial.println("3. Empty syringe completely");
  Serial.println("4. Fill syringe completely (200 CC)");
  Serial.println();
}

//Function to empty syringe completely.
void EmptySyringe()
{
  digitalWrite(dir, HIGH); //Pull direction pin high to move in "reverse"
  for(x= 0; x<position; x++)  //Loop the backward stepping till syringe is emptied.
  {
    digitalWrite(stp,HIGH); //Trigger one step
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(2);
  }

  position = 0; //Set position to 0.

  //Reprint out menu
  Serial.println();
  Serial.println("Enter number for control option:");
  Serial.println("1. Fill syringe certain amount");
  Serial.println("2. Empty syringe certain amount");
  Serial.println("3. Empty syringe completely");
  Serial.println("4. Fill syringe completely (200 CC)");
  Serial.println();
}


//Function to fill syringe completely.
void FillSyringe()
{
  Serial.println("Filing syringe completely.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
    for(x= 0; x<maximum*constant-position; x++){  //Loop the forward stepping till the syringe is filled.
      digitalWrite(stp,HIGH); //Trigger one step forward
      delay(1);
      digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
      delay(2);
    }

  position = maximum*constant; //Set position to the maximum

  //Reprint out menu
  Serial.println();
  Serial.println("Enter number for control option:");
  Serial.println("1. Fill syringe certain amount");
  Serial.println("2. Empty syringe certain amount");
  Serial.println("3. Empty syringe completely");
  Serial.println("4. Fill syringe completely (200 CC)");
  Serial.println();
}

//Reset Easy Driver pins to default states
void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}
