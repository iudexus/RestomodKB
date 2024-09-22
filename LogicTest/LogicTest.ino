/*here is a simple, single button logic test which can check a keypress 
with the digital scanning method, only on one pin*/

#include "Keyboard.h"

//define row and column pins as variables for quick and easy changes
const int rowpin = 1;
const int colpin = 21;

void setup() {
  //set up a little visual output.
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize keyboard control
  Keyboard.begin();

  //this is the'row' pin
  pinMode(rowpin, OUTPUT); //no need to change pin state in loop if starting as output
  digitalWrite(rowpin, HIGH); //rows start set to HIGH (5V)

  //this is the column pin
  pinMode(colpin, INPUT_PULLUP); //columns pull up (also HIGH)

  //star with LED on to indicate life
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  //this is the 'scanning' phase
  digitalWrite(rowpin, LOW);

  //this checks if column pin was pulled low with the above action
  if (digitalRead(21) == LOW) {
    
    digitalWrite(LED_BUILTIN, LOW); //visual que

    delay(160); //debounce to avoid doubling keypress

    //cleanup by setting LED back to ON and row back to HIGH
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(1, HIGH);

    //type some output
    char inChar = 'C';
    Keyboard.write(inChar);
  }
}
