/*here is a simple, single button logic test which can check a keypress 
with the digital scanning method, only on one pin*/

#include "Keyboard.h"

void setup() {
  //set up a little visual output.
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize keyboard control
  Keyboard.begin();

  //this is the'row' pin
  pinMode(1, OUTPUT); //no need to change pin state in loop if starting as output
  digitalWrite(1, HIGH); //rows start set to HIGH (5V)

  //this is the column pin
  pinMode(21, INPUT_PULLUP); //columns pull up (also HIGH)

  //star with LED on to indicate life
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  //this is the 'scanning' phase
  digitalWrite(1, LOW);

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
