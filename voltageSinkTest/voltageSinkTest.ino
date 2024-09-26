/*
This simple test is designed to test voltage sinking on an LED with an arduino Uno 
to see if the board can take it and to see if it can be done.  

Wiring:
With a breadboard, placed 1 momentary switch with 1 leg on pin 13, pulled high and 
one leg to ground for maximum visibility using onboard LEDs.  For LED, useing pin 3
as 5v out set high with yellow LED connected, next 2 330 ohm resistors in prrallel 
for 165 ohm total, then wired back to pin 2, which is also an output, set high.  
Setting pin 2 low will "sink" voltage into ground, resulting in curent draw, and 
lighting the LED. Sending voltage on both sides of the LED wont affect anything
since the LED is a diode. 
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 13;  // the number of the pushbutton pin
const int ledPin = 3;    // the number of the LED pin
const int sinkPin = 2;    //the number of the pin to be pulled LOW

// variables will change:
int buttonState = 0;  // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  //set high to supply voltage for sinking
  digitalWrite(ledPin, HIGH);
  // initialize the pushbutton pin as an input starting high since it's wired to gnd.
  pinMode(buttonPin, INPUT_PULLUP);
  //initialize the sink pin as in
  pinMode(sinkPin, OUTPUT);
  //set sink pin high (the LED is a diode, so there is no risk. 
  digitalWrite(sinkPin, HIGH);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    // turn LED on:
    digitalWrite(sinkPin, LOW);
    //keep the LED on for a bit         
    delay(3000);

    digitalWrite(sinkPin, HIGH);  
  }
}