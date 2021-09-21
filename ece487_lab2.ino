/*    
 *     
 *     
        Samuel Courtney 
        CWID: 11682010  

          Lab Assignment 2

          Assignment Description:
            This lab assignment is an extension of the first lab assignment. In this program LEDs will be toggled using a switch. 
            The count of state changes recorded by toggle switch will be recorded and displayed with no rollover in decimal and 
            rollover in hexidecimal. A seven segment display will show the number of times the switch was toggled with rollover.            
 *          
 *
 */

//Setup loop that initializes the program, serial monitor and some pinouts
void setup() {
  
  Serial.setTimeout(50);  // Makes the LEDs turn off faster
  Serial.begin(115200);   // Sets Baud Rate (bps) to 115200

  delay(100);             // Protects against garbage characters upon upload
  pinMode(2,OUTPUT);      //Seven segment 'a' diode for common cathode (part #5161AG-1)
  pinMode(3,OUTPUT);      //Seven segment 'b' ..
  pinMode(4,OUTPUT);      //Seven segment 'c' ..
  pinMode(5,OUTPUT);      //Seven segment 'd' .. 
  pinMode(6,OUTPUT);      //Seven segment 'e' ..
  pinMode(7,OUTPUT);      //Seven segment 'f' .. 
  pinMode(9,OUTPUT);      //Seven segment 'g' ..
  pinMode(10,OUTPUT);     //Seven segment '.' ..

  while (! Serial);                // The progam will wait until the serial is ready
  for(int i = 0; i <= 35; i++) {   // Printing new lines to get rid of trash charaters in the serial monitor
        Serial.println(" ");
    }

  // Once clean & ready, prompt user with instructions & initial count:
  Serial.println("Toggle the switch to blink the LEDs");
  Serial.println("The initial count is: (decimal) 0 (hex) 0");
}

//Main function of code that preforms the desired objectives described in the Assignment Description.
void loop() {
  // These variables are used to read the user input, store the blink rate of the LEDs, and store the current state of the LEDs.
  int togglePin = 8;                // Toggle switch pin number
  int toggleState = LOW;            // Toggle switch set to LOW
  int stateChangeCount = 0;         // Primary change of state count - used for no rollover cases
  int rollCount = 0;                // Secondary change of state count - used for rollover cases.
  int previousState = LOW;          // Records last state of toggle switch
  int flag = 0 ;                    // A flag to determine initial switch state
  int offboardLED = 12;             // Off-board LED pin setting.
  int onboardLED  = LED_BUILTIN;    // On-board LED pin setting.
  int ledState1   = LOW;            // The inital state of the off-board LED.
  int ledState2   = LOW;            // The inital state of the on-board LED.
  long onTime     = 1000;           // The milliseconds of on-time for the on-board LED.
  long offTime    = 1000;           // The milliseconds of off-time for the on-board LED.
  unsigned long previousTime = 0;   // This variable will store last time the state machine was updated
  pinMode(offboardLED, OUTPUT);     // Sets the offboard digital led as an output
  pinMode(onboardLED,  OUTPUT);     // Sets the onboard digital pin as an output
  pinMode(togglePin, INPUT);        // Sets the offboard digital pin as an input

  displayCount(rollCount);  // Initializes the seven segment display to the desired number '0' through check function

  toggleState = digitalRead(togglePin); // Records the current state of the toggle switch
  previousState = toggleState;          // stores the previous state as the current state for first iteration

    //initial while loop to blink the LEDs based on switch condition (if switch high but state has not changed etc..)
    while(toggleState != LOW) {                     // Condition to end loop if toggled OFF
      unsigned long currentTime = millis();         // Storage of LED cycle time data
      toggleState = digitalRead(togglePin);
        /* On-board LED */
        if((ledState2 == HIGH) && (currentTime - previousTime >= onTime)) {
          ledState1 = HIGH;                         // Turn on-board LED on
          ledState2 = LOW;                          // Turn off-board LED off
          previousTime = currentTime;               // Remember the time when the LEDs were turned on/off
          digitalWrite(offboardLED, ledState1);     // Update the off-board LED's state
          digitalWrite(onboardLED, ledState2);      // Update the on-board LED's state
        }
        else if ((ledState2 == LOW) && (currentTime - previousTime >= offTime)) {
          ledState1 = LOW;                          // Turn on-board LED off
          ledState2 = HIGH;                         // Turn off-board LED on
          previousTime = currentTime;               // Remember the time when the LEDs were turned on/off
          digitalWrite(offboardLED, ledState1);     // Update the off-board LED's state
          digitalWrite(onboardLED, ledState2);      // Update the on-board LED's state
        }
      displayCount(rollCount);  // Update the seven segment display 
      flag = 1;                // Set flag to one to indicate switch state change  
    }     
  ledState1 = LOW;
  ledState2 = LOW;
  digitalWrite(offboardLED, ledState1);         // Update the off-board LED's state LOW
  digitalWrite(onboardLED, ledState2);          // Update the on-board LED's state LOW

    while(1) { 
    //if the switch is high before the state is changed, update the state
      if(flag == 1) { 
        rollCount++;                  // Increment the no-rollover statecount
        stateChangeCount++;           // Increment the rollover statecount
        displayCount(rollCount);       // Update the seven segment display 
        flag = 0;                     // Flag reset
      
        Serial.println("\nLED Cycle OFF. Toggle the switch to start the loop.");
        Serial.print("count = ");
        Serial.print("(decimal) ");
        Serial.print(stateChangeCount); //prints the rollover statecount
        Serial.print(" (hex) ");
        Serial.print(rollCount, HEX);   //prints the no-rollover statecount in HEX
      }

      previousState = toggleState;
      toggleState = digitalRead(togglePin);
  
      if(toggleState == HIGH && previousState != HIGH) { delay(300); } //Protect against switch bounce
    
        if(toggleState == HIGH && previousState != HIGH) {             // When switch is ON condition..
      
          delay(50);
        
          previousState = HIGH;
          stateChangeCount++;
          Serial.println("\nLED Cycle ON. Toggle the switch to stop the loop.");
          Serial.print("count = ");
          Serial.print("(decimal) ");
          Serial.print(stateChangeCount); //prints the rollover statecount
          Serial.print(" (hex) ");
          rollCount++;
            if(rollCount >= 16){          //if the count >= 16 roll
              rollCount = 0;
            }
          Serial.print(rollCount, HEX);     //prints the no-rollover statecount in HEX
        
            while(toggleState != LOW) {                   // Blinks the LED
              unsigned long currentTime = millis();       // Storage of LED cycle time data
              toggleState = digitalRead(togglePin);       // Reads switch
              
              /* On-board LED */
              if((ledState2 == HIGH) && (currentTime - previousTime >= onTime))
              {
                ledState1 = HIGH;                         // Turn on-board LED on
                ledState2 = LOW;                          // Turn off-board LED off
                previousTime = currentTime;               // Remember the time when the LEDs were turned on/off
                digitalWrite(offboardLED, ledState1);     // Update the off-board LED's state
                digitalWrite(onboardLED, ledState2);      // Update the on-board LED's state
              }
              else if ((ledState2 == LOW) && (currentTime - previousTime >= offTime))
              {
                ledState1 = LOW;                          // Turn on-board LED off
                ledState2 = HIGH;                         // Turn off-board LED on
                previousTime = currentTime;               // Remember the time when the LEDs were turned on/off
                digitalWrite(offboardLED, ledState1);     // Update the off-board LED's state
                digitalWrite(onboardLED, ledState2);      // Update the on-board LED's state
              }
            displayCount(rollCount); // Update the seven segment display 
          }
        }

        if(toggleState == LOW && previousState != LOW) { delay(300); }
    
          if(toggleState == LOW && previousState != LOW) { 
            ledState1 = LOW;
            ledState2 = LOW;
            digitalWrite(offboardLED, ledState1);         // Update the off-board LED's state
            digitalWrite(onboardLED, ledState2);          // Update the on-board LED's state

          if(previousState == HIGH && toggleState == LOW) {
            delay(250);
            stateChangeCount++;
            Serial.println("\nLED Cycle OFF. Toggle the switch to start the loop.");
            Serial.print("count = ");
            Serial.print("(decimal) ");
            Serial.print(stateChangeCount); // norollover
            Serial.print(" (hex) ");
            previousState = LOW;
            rollCount++;
            if(rollCount >=16){ //checks rollover
            rollCount = 0;
            }
          Serial.print(rollCount, HEX); //prints in hex
          displayCount(rollCount); //displays number
        }
      }
    } 
  }


// This function will take in an interger value and display the value on the seven segment display
void displayCount(int rollCount1) {

   if(rollCount1 == 0){ //displays zero
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, LOW);
    }
    if(rollCount1 == 1){ //displays one
       digitalWrite(2, LOW);
       digitalWrite(3, HIGH);
       digitalWrite(4, HIGH);
       digitalWrite(5, LOW);
       digitalWrite(6, LOW);
       digitalWrite(7, LOW);
       digitalWrite(9, LOW);
    }
    if(rollCount1 == 2) { //displays two
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH);
    }
    if(rollCount1 == 3) { //displays three
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH);
    }
    if(rollCount1 == 4) { //displays four
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
    }
    if(rollCount1 == 5) { //displays five
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
    }
    if(rollCount1 == 6) { //displays six
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
    }
    if(rollCount1 == 7) { //displays seven
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(7, LOW);
      digitalWrite(9, LOW);
    }
    if(rollCount1 == 8) { //displays eight
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
    }
    if(rollCount1 == 9) { //displays nine
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
    }
    if(rollCount1 == 10) { //displays A.
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(rollCount1 == 11) { //displays b.
      digitalWrite(2, LOW);
      digitalWrite(3, LOW);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(rollCount1 == 12) { //displays C.
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(10, HIGH);
    }
    if(rollCount1 == 13) { //displays d.
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      digitalWrite(4, HIGH);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(rollCount1 == 14) { //displays E.
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
    if(rollCount1 == 15) { //displays F.
      digitalWrite(2, HIGH);
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
    }
   return;
 }
