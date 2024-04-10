// i2c master writer - state machine with array variables, 3+ arduinos
// use new line
// 3/12/24

/*
3/14/24
- get rid of useless parts like the last flag - done
- look at copy paste parts
- shorten if statements, don't need all of the statements - done
- turn bool variables to int flag variable that changes from 1 to 7
4/3/24
- char to int : x-'0'
- Wire.endTransmission didn't work until all arduinos were connected or the wires of the 3rd arduino were disconnected
- added n_VCS and configuration area
- to do: 
  - update first question so that it doesn't ask to choose from 1-3 but instead can be any number with n_VCS
  - if statement or function to see if it is a number before \n (line 70)
*/



/*
configuration:
- n_VCS stores the number of VCSs connected
- vcsaddress stores the I2C addresses for each VCS
- for the user: enter the values for n_VCS and vcsaddress before running code
*/


const int n_VCS = 3; // look at configuration notes before running
int vcsaddress[n_VCS] = {8, 9, 10}; // vcs 1 corresponds to address 8, vcs 2 corresponds to 9

#include <Wire.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
float vol = 0; //volume of syringe while running
float vols[n_VCS] = {};

void setup() {
  Wire.begin(); // join I2C bus (address optional for master)
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  for(int i = 0; i < n_VCS; i++)
  {
    vols[i] = -1; // start with negative/invalid numbers, will later store the updated volumes
  }

}

bool vcsuserprompt1 = false; // which vcs?
bool vcsuserinput1 = false;
bool userprompt1 = false; // how much water is currently in syringe
bool userinput1 = false;
bool userprompt2 = false; // enter amt to fill or dispense
bool userinput2 = false;
bool yesnoprompt = false; //continue or change vcs?
bool yesnoinput = false;

int vcs = 0;

void loop() {

  if((vcsuserprompt1 == false) && (vcsuserinput1 == false))
  {
    Serial.print("Which VCS would you like to control? ");
    vcsuserprompt1 = true;
  }

  if((vcsuserprompt1 == true) && stringComplete && (userprompt1 == false))
  {
    Serial.println(inputString);
    // if statement or function to see if it is a number before \n
    String chooseVCS; // will be inputString minus \n
    for(int i = 0; i < inputString.length(); i++)
    {
      if(isDigit(inputString[i]))
      {
        chooseVCS += inputString[i];
      }
    }
    if(chooseVCS.toInt() > 0 && chooseVCS.toInt() <= n_VCS) //(inputString == "1\n") || (inputString == "2\n") || (inputString == "3\n")
    {
      vcsuserinput1 = true;
      char x = inputString.charAt(0);
      vcs = x - '0'; // char to int
      clearString();
      if(vols[vcs-1] > 0)
      {
        vol = vols[vcs-1];
        userprompt1 = true;
        userinput1 = true; // skips the how much currently in syringe question
      }
    }
    else
    {
      Serial.print("Enter a valid VCS number: ");    
      clearString();
    }
  
  }


  if((vcsuserinput1 == true) && (userprompt1 == false))
  {    
    userprompt1 = true;
    Serial.print("How much water is currently in the syringe? ");
    clearString();
  }
  
  if((vcsuserinput1 == true) && (userprompt1 == true) && (userinput1 == false) && stringComplete)
  {
    if (stringComplete && isFloat(inputString))
    {
      vol = inputString.toFloat();
      if(vol>200||vol<0) 
      {
        Serial.print("Enter a valid number: ");
        clearString();
      }
      else
      {
        userinput1 = true;
        clearString();
      }
    }
    else
    {
      Serial.print("Enter a numerical value: ");
      clearString();
    }
  }
  
  if((userinput1 == true) && (userprompt2 == false))
  {
    Serial.print("Enter the amount of water you want to fill or dispense (if dispensing, write '-' before amount): ");
    userprompt2 = true;
    clearString();
  }

  if((userprompt2 == true) && (userinput2 == false))
  {
    if(stringComplete)
    {
      if(isFloat(inputString))
      {
        //getting the new volume in syringe after fill/dispense
        float changeInVol = inputString.toFloat();

        // make sure it's between 0 and 200
        if(vol+changeInVol < 0 || vol+changeInVol > 200)
        {
          Serial.print("Please enter a valid amount to fill or dispense: ");
          clearString();
        }
        else
        {
          vol += changeInVol;
          char buf[12];  // The character buffer to store the converted float value
          dtostrf(vol, 10, 3, buf);
          vols[vcs - 1] = vol;
          
          //Serial.println(vols[vcs-1]); // to check, delete later
          Wire.beginTransmission(vcsaddress[vcs - 1]); // transmit to device #8, vcsaddress[vcs - 1]
          Wire.write(buf);  
          Wire.endTransmission();    // stop transmitting
          userinput2 = true;
          clearString();
        }
      }
      else
      {
        Serial.print("Please enter a numerical value: ");
        clearString(); 
      }
    }
  }

//make if statement to see if u want to continue same vcs or change
  if((userinput2 == true) && (yesnoprompt == false))
  {
    Serial.print("Would you like to continue using the same VCS? Enter yes or no: ");
    yesnoprompt = true;
    clearString(); 
  }

  if((yesnoprompt == true))
  {
    if(stringComplete)
    {
      if(inputString == "yes\n")
      {
        Serial.println(inputString);
        userprompt2 = false;
        userinput2 = false;
        yesnoprompt = false;
        yesnoinput = false;
        clearString();
      }
      else if(inputString == "no\n")
      {
        Serial.println(inputString);

        vcsuserprompt1 = false;
        vcsuserinput1 = false;
        userprompt1 = false;
        userinput1 = false;
        userprompt2 = false;
        userinput2 = false;
        yesnoprompt = false;
        yesnoinput = false;
        clearString(); 
      }
      else
      {
        Serial.print("Please enter yes or no: ");
        clearString();
      }
    }
  }
}

void clearString()
{
  //clear the string
  inputString = "";
  stringComplete = false; 
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

bool isFloat(String num) //add if statement for \n
{
  Serial.println(num);
  //variable isNum
  bool isNum = true;
  if(num.charAt(num.length()-1) == '\n')
  {
    for(int i = 0; i < num.length()-1; i++)
    {
      if((isDigit(num.charAt(i))||num.charAt(i)=='-'||num.charAt(i)=='.'))
      {
       isNum = true;
      }
      else
      {
        return false;
      }
    }
  }
  else
  {
    for(int i = 0; i < num.length(); i++)
    {
      if((isDigit(num.charAt(i))||num.charAt(i)=='-'||num.charAt(i)=='.'))
      {
        isNum = true;
      }
      else
      {
        return false;
      }
    }
  }  
  return true;
}