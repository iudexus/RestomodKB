//Jameco Electronics Keyboard Model JE1015 Restoration and Modification with Teensy 2.0 using analog matrix read

#include <Keyboard.h>

//Start setup of keyboard matrixby defining number of columns and rows
const int numrows = 8;  //pins connected to on-board 1k pullup resistors
const int numcols = 11; //cins connected to "coloms"/traces out to keys

//define the specific pins for each coordinate set
const int rowpins[numrows] = {0,1,2,3,4,5,6,7};
const int colpins[numcols] = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10};

//manually define macros as variables with decimal values
/*
#define KEY_1  225
#define KEY_2  226
#define KEY_3  227
#define KEY_4  228
#define KEY_5  229
#define KEY_6  230
#define KEY_7  231
#define KEY_8  232
#define KEY_9  233
#define KEY_0  234
#define KEYPAD_PLUS  223
#define KEYPAD_MINUS  222
#define KEY_PERIOD  235
#define KEY_NUM_LOCK  219

#define KEY_ESC  177
#define KEY_F1  194
#define KEY_F2  195
#define KEY_F3  196
#define KEY_F4  197
#define KEY_F5  198
#define KEY_F6  199
#define KEY_F7  200
#define KEY_F8  201
#define KEY_F9  202
#define KEY_F10  203
#define KEY_F11  204
#define KEY_F12  205

#define KEY_RETURN  176
#define KEY_BACKSPACE  178
#define KEY_CAPS_LOCK  193
#define KEY_TAB  179

#define KEY_LEFT_SHIFT  129
#define KEY_RIGHT_SHIFT  133
#define KEY_LEFT_CTRL  128
#define KEY_LEFT_ALT  130

#define KEY_PRINT_SCREEN  206

//Collom 11 (the last one) doesnt have all 8 characters, so representing empties w/ '\0' */
uint16_t keymap[numrows][numcols] = { //set variable type to uint16_t instead of char to hold values over 255
  {'c', KEY_PERIOD, KEY_0, KEY_PRINTSCREEN, '\\', '.', 'm', 'b', 'z', KEY_F7, '\0'}, //0
  {'s', KEY_F12, KEY_1, '`', ';', 'k', 'h', 'f', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'x', KEYPAD_PLUS, KEY_5, KEY_RIGHT_SHIFT, '/', ',', 'n', 'v', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'d', KEY_3, KEY_2, KEY_RETURN, '\'', 'l', 'j', 'g', KEY_F10, 'a', KEY_F8}, //3
  {'2', KEY_F11, KEY_NUM_LOCK, '=', '0', '8', '6', '4', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'w', KEY_9, KEY_7, ']', 'p', 'i', 'y', 'r', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'e', KEY_6, KEY_4, ' ', '[', 'o', 'u', 't', 'q', KEY_F4, '\0'}, //6
  {'3', KEYPAD_MINUS, KEY_8, KEY_BACKSPACE, '-', '9', '7', '5', '1', KEY_F2, '\0'} //7
};

uint16_t specialkeymap[numrows][numcols] = { //set variable type to uint16_t instead of char to hold values over 255
  {'C', KEY_PERIOD, KEY_0, KEY_PRINTSCREEN, '\\', '.', 'M', 'B', 'Z', KEY_F7, '\0'}, //0
  {'S', KEY_F12, KEY_1, '`', ';', 'K', 'H', 'F', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'X', KEYPAD_PLUS, KEY_5, KEY_RIGHT_SHIFT, '/', ',', 'N', 'V', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'D', KEY_3, KEY_2, KEY_RETURN, '\'', 'L', 'J', 'G', KEY_F10, 'A', KEY_F8}, //3
  {'2', KEY_F11, KEY_NUM_LOCK, '=', '0', '8', '6', '4', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'W', KEY_9, KEY_7, ']', 'P', 'I', 'Y', 'R', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'E', KEY_6, KEY_4, ' ', '[', 'O', 'U', 'T', 'Q', KEY_F4, '\0'}, //6
  {'3', KEYPAD_MINUS, KEY_8, KEY_BACKSPACE, '-', '9', '7', '5', '1', KEY_F2, '\0'} //7
};

//define special cases as boolean flags
bool shift = FALSE;
bool capslock= FALSE;
bool numlock = TRUE;

void setup() {
  //initialize keyboard control
  Keyboard.begin();

  //initialize serial console to be activated for debugging - remember to set Tools > USB Type to Serial+kb+mouse+joystk
  Serial.begin(9600);

  //set up rows as dry inputs
  for (int i = 0; i < numrows; i++) {
    pinMode(rowpins[i], OUTPUT);
    digitalWrite(rowpins[i], LOW); //set LOW to scan later with HIGH
  }

  //set up analog columns as inputs
  for (int i = 0; i < numcols; i++) {
    pinMode(colpins[i], INPUT);
  }
  //delay to not immediately jump into opperation
  delay(2000);
}

void loop() {
  
  for (int row = 0; row < numrows; row++) {
    //set rowpin HIGH to apply voltage and scan row
    digitalWrite(rowpins[row], HIGH);

    //read the state of each column
    for (int col = 0; col < numcols; col++) {
      
      //capture active column (analog)
      if (analogRead(colpins[col]) >= 1002) { //can as smooth analog read here  to help debounce and input verification
      
      //correspond where key is pressed at row and col
      char key = keymap[row][col]; //variable type set to uint16_t to hold macros - treat as char

      //DEBUGGING - uncomment below 2 lines to capture and send keystrokes via serial console
      Serial.print("Key pressed: ");
      Serial.println(key);  // Print the keystroke over serial - comment below line to stop sending via keystroke
      Serial.print("row: ");
      Serial.println(row);  // Print the keystroke over serial - comment below line to stop sending via keystroke
      Serial.print("col: ");
      Serial.println(col);  // Print the keystroke over serial - comment below line to stop sending via keystroke

      Keyboard.write(key);  //send keypress to PC
      
      delay(170);  //debounce to avoid repeat
      }
    }
        //set row back to LOW after scanning to not re-read
    digitalWrite(rowpins[row], LOW);
  }
}