/*here is a simple, single button logic test which can check a keypress by applying 
voltage to the would-be row pin and listening for analog fluctuation on the column*/

#include "Keyboard.h"

//define row and column pins as variables for quick and easy changes
const int rowpin = 1;
const int colpin = A0;

//unneeded global definition - TEST AND REMOVE
//int keypress = 0;

void setup() {
  //set up a little visual output.
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize keyboard control
  Keyboard.begin();
  Serial.begin(9600);
  //this is the'row' pin
  pinMode(rowpin, OUTPUT); //no need to change pin state in loop if starting as output
  digitalWrite(rowpin, LOW); //rows start set to HIGH (5V) for digital and LOW (0V) for analog

  //this is the column pin
  pinMode(colpin, INPUT); //columns pull up (also HIGH)

  //star with LED on to indicate life
  digitalWrite(LED_BUILTIN, HIGH);

  delay(4000);
}

void loop() {

  //this is the 'scanning' phase
  //pinMode(rowpin, OUTPUT); //no need to change pin state in loop if starting as output
  //digitalWrite(rowpin, LOW);  
  digitalWrite(rowpin, HIGH);

  //this checks if column pin was pulled low with the above action
  int keypress = smoothAnalogRead(colpin);
  if (keypress >= 1001) {
    
    digitalWrite(LED_BUILTIN, LOW); //visual que

    //delay(200); //debounce to avoid doubling keypress

    //cleanup by setting LED back to ON and row back to HIGH
    digitalWrite(LED_BUILTIN, HIGH);
    
    Serial.print("analog value: ");
    Serial.println(keypress);
    //type some output
    char inChar = 'C';
    int writect = 0;
    Keyboard.write(inChar);
    writect++;
    delay(10);
    char outChar = inChar;
    if (writect != 0 && outChar == inChar) {
      delay(200);
      return;
    }
    delay(100);
  }
delay(10);
}
//FUNCTION smoothAnalogRead
int smoothAnalogRead(int A0) {
  int numReadings = 16;  // Number of readings to average
  long total = 0;

  for (int i = 0; i < numReadings; i++) {
    total += analogRead(A0);  // Add up the readings
    delay(7);  // Small delay between readings
  }
  return total / numReadings;  // Return the average reading
}
