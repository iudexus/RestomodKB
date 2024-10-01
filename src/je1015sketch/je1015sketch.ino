//Jameco Electronics Keyboard Model JE1015 Restoration and Modification with Teensy 2.0 using analog matrix read

#include <Keyboard.h>

//Start setup of keyboard matrixby defining number of columns and rows
const int numrows = 8;  //pins connected to on-board 1k pullup resistors
const int numcols = 11; //cins connected to "coloms"/traces out to keys

//define the specific pins for each coordinate set
const int rowpins[numrows] = {1,2,3,4,5,6,7,8};
const int colpins[numcols] = {A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10};

//define bitwise indicators for selecting active keymap for typing - note to self: |=, or, turns on through addition : &= ~, and not, turns off by specifying : ^=, xor, toggles with exclusion
#define numlk 0x01 //bit 0
#define caps 0x02 //bit 1
#define shift 0x04 //bit 2
//shift nolonger defines this bit, instead allowing the computer to handle shift cases to make capslk interaction easier - 2am revelation

//this modifier's state will index which of the following keymaps to get
byte activeMatrix = 1;  //numlock on by default

//this array lists the states of the keyboard given current modifier in order to read from state bit. 
const char (*matrices[8])[numcols] = {
  noState, //0: no state, 0 bit
  numlkDefault, //1: numlk active, 1 bit
  justCap, //2: caps active, 2 bit
  numCap, //3: num + caps, 1 + 2 bit
  justShift, //4: shift active, 4 bit
  numShift, //5: num + shift, 1 + 5 bit
  CapShift, //6: caps + shift, 2 + 4 bit
  numCapShift, //7: all states, all bits
};

/*

//manually define macros as variables with decimal values

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
#define KEY_KP_ASTERISK  221
#define KEY_PAGE_UP  211
#define KEY_PAGE_DOWN  214
#define KEY_DELETE  212
#define KEY_INSERT  209
#define KEY_END  213
#define KEY_HOME  210
#define KEY_UP_ARROW  218
#define KEY_DOWN_ARROW  217
#define KEY_LEFT_ARROW  216
#define KEY_RIGHT_ARROW  215
#define KEY_KP_ENTER  224

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

*/

//Collom 11 (the last one) doesnt have all 8 characters, so representing empties w/ '\0' --normal state no caps numlk ON
uint16_t noState[numrows][numcols] = { //set variable type to uint16_t instead of char to hold values over 255
  {'c', KEY_DELETE, KEY_INSERT, KEY_PRINTSCREEN, '\\', '.', 'm', 'b', 'z', KEY_F7, '\0'}, //0
  {'s', KEY_F12, KEY_END, '`', ';', 'k', 'h', 'f', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'x', KEY_KP_ENTER, KEY_5, KEY_RIGHT_SHIFT, '/', ',', 'n', 'v', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'d', KEY_PAGE_DOWN, KEY_DOWN_ARROW, KEY_RETURN, '\'', 'l', 'j', 'g', KEY_F10, 'a', KEY_F8}, //3
  {'2', KEY_F11, KEY_NUM_LOCK, '=', '0', '8', '6', '4', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'w', KEY_PAGE_UP, KEY_HOME, ']', 'p', 'i', 'y', 'r', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'e', KEY_RIGHT_ARROW, KEY_LEFT_ARROW, ' ', '[', 'o', 'u', 't', 'q', KEY_F4, '\0'}, //6
  {'3', KEYPAD_MINUS, KEY_UP_ARROW, KEY_BACKSPACE, '-', '9', '7', '5', '1', KEY_F2, '\0'} //7
};

uint16_t numlkDefault[numrows][numcols] = { //set variable type to uint16_t instead of char to hold values over 255
  {'c', KEY_PERIOD, KEY_0, KEYPAD_ASTERIX, '\\', '.', 'm', 'b', 'z', KEY_F7, '\0'}, //0
  {'s', KEY_F12, KEY_1, '`', ';', 'k', 'h', 'f', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'x', KEYPAD_PLUS, KEY_5, KEY_RIGHT_SHIFT, '/', ',', 'n', 'v', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'d', KEY_3, KEY_2, KEY_RETURN, '\'', 'l', 'j', 'g', KEY_F10, 'a', KEY_F8}, //3
  {'2', KEY_F11, KEY_NUM_LOCK, '=', '0', '8', '6', '4', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'w', KEY_9, KEY_7, ']', 'p', 'i', 'y', 'r', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'e', KEY_6, KEY_4, ' ', '[', 'o', 'u', 't', 'q', KEY_F4, '\0'}, //6
  {'3', KEYPAD_MINUS, KEY_8, KEY_BACKSPACE, '-', '9', '7', '5', '1', KEY_F2, '\0'} //7
};

/*
uint16_t shiftkeymap[numrows][numcols] = { //createing a special case matrix to handle shift, capslk, and numlk OFF
  {'C', KEY_DELETE, KEY_INSERT, KEYPAD_ASTERIX, '|', '>', 'M', 'B', 'Z', KEY_F7, '\0'}, //0
  {'S', KEY_F12, KEY_END, '~', ':', 'K', 'H', 'F', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'X', KEYPAD_PLUS, KEY_5, KEY_RIGHT_SHIFT, '?', '<', 'N', 'V', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'D', KEY_PAGE_DOWN, KEY_DOWN_ARROW, KEY_RETURN, '"', 'L', 'J', 'G', KEY_F10, 'A', KEY_F8}, //3
  {'@', KEY_F11, KEY_NUM_LOCK, '+', ')', '*', '^', '$', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'W', KEY_PAGE_UP, KEY_HOME, '}', 'P', 'I', 'Y', 'R', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'E', KEY_RIGHT_ARROW, KEY_LEFT_ARROW, ' ', '{', 'O', 'U', 'T', 'Q', KEY_F4, '\0'}, //6
  {'#', KEYPAD_MINUS, KEY_UP_ARROW, KEY_BACKSPACE, '_', '(', '&', '%', '!', KEY_F2, '\0'} //7
};
*/

uint16_t justCap[numrows][numcols] = { //set variable type to uint16_t instead of char to hold values over 255
  {'C', KEY_DELETE, KEY_INSERT, KEY_PRINTSCREEN, '\\', '.', 'M', 'B', 'Z', KEY_F7, '\0'}, //0
  {'S', KEY_F12, KEY_END, '`', ';', 'K', 'H', 'F', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'X', KEY_KP_ENTER, KEY_5, KEY_RIGHT_SHIFT, '/', ',', 'N', 'V', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'B', KEY_PAGE_DOWN, KEY_DOWN_ARROW, KEY_RETURN, '\'', 'L', 'J', 'G', KEY_F10, 'A', KEY_F8}, //3
  {'2', KEY_F11, KEY_NUM_LOCK, '=', '0', '8', '6', '4', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'W', KEY_PAGE_UP, KEY_HOME, ']', 'P', 'I', 'Y', 'R', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'E', KEY_RIGHT_ARROW, KEY_LEFT_ARROW, ' ', '[', 'O', 'U', 'T', 'Q', KEY_F4, '\0'}, //6
  {'3', KEYPAD_MINUS, KEY_UP_ARROW, KEY_BACKSPACE, '-', '9', '7', '5', '1', KEY_F2, '\0'} //7
};

uint16_t numCap[numrows][numcols] = { //set variable type to uint16_t instead of char to hold values over 255
  {'C', KEY_PERIOD, KEY_0, KEY_PRINTSCREEN, '\\', '.', 'M', 'B', 'Z', KEY_F7, '\0'}, //0
  {'S', KEY_F12, KEY_1, '`', ';', 'K', 'H', 'F', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'X', KEY_KP_ENTER, KEY_5, KEY_RIGHT_SHIFT, '/', ',', 'N', 'V', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'D', KEY_3, KEY_2, KEY_RETURN, '\'', 'L', 'J', 'G', KEY_F10, 'A', KEY_F8}, //3
  {'2', KEY_F11, KEY_NUM_LOCK, '=', '0', '8', '6', '4', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'W', KEY_9, KEY_7, ']', 'P', 'I', 'Y', 'R', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'E', KEY_6, KEY_4, ' ', '[', 'O', 'U', 'T', 'Q', KEY_F4, '\0'}, //6
  {'3', KEYPAD_MINUS, KEY_8, KEY_BACKSPACE, '-', '9', '7', '5', '1', KEY_F2, '\0'} //7
};

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

  //LED pin setup - this board requires holding LED voltage high, then "sinking" to GND
  pinMode(22, OUTPUT) //power pin
  digitalWrite(22, HIGH) //5v out
  //initialize scrlk LED
  pinMode(24, OUTPUT) //scrlk sink pin
  digitalWrite(24, HIGH) //5v out to back of diode
  //initialize capslk LED
  pinMode(10, OUTPUT) //caplk sink pin
  digitalWrite(10, HIGH) //5v out to back of diode
  //initialize numlk LED
  pinMode(9, OUTPUT) //numlk sink pin
  digitalWrite(9, HIGH) //5v out to back of diode
}

void loop() {

//begin scanning keys
  for (int row = 0; row < numrows; row++) {
    //set rowpin HIGH to apply voltage and scan row
    digitalWrite(rowpins[row], HIGH);

    //read the state of each column
    for (int col = 0; col < numcols; col++) {
      
      //capture active column (analog)
      if (analogRead(colpins[col]) >= 1002) { 
        
        //correspond where key is pressed at row and col in the active matrix
        uint16_t key = matrices[activeMatrix][row][col]; //variable type set to uint16_t to hold macros - treat as char
        if (key == KEY_NUM_LOCK || key == KEY_CAPS_LOCK) {
          numCapsChanger(key); //pass in modifier key to the function
        }
        else if (key == KEY_LEFT_SHIFT || key == KEY_RIGHT_SHIFT) {
          siftHandler(col, key); //pass in the active shift pin to listen for release
          return;
        }
        else if (key == KEY_LEFT_ALT || key == KEY_LEFT_CTRL) {
          comboHandler(key) //pass in the combo trigger to start combo cycle
        }
        else if (key != '/0') { //check for null character
          Keyboard.write(key);  //send keypress to PC
        }
        digitalWrite(rowpins[row], LOW); //clean up immediately after sending data

        //DEBUGGING - uncomment below lines to capture and send keystrokes via serial console
        Serial.print("Key pressed: ");
        Serial.println(key);  // Print the keystroke over serial - comment below line to stop sending via keystroke
        Serial.print("row: ");
        Serial.println(row);  // Print the keystroke over serial - comment below line to stop sending via keystroke
        Serial.print("col: ");
        Serial.println(col);  // Print the keystroke over serial - comment below line to stop sending via keystroke
        
        delay(170);  //debounce to avoid repeat kaystroke
      }
    }
  }
  if (activeMatrix == 1) {
      digitalWrite(9, LOW) //GND this pin to pull voltage across numlk LED
      digitalWrite(10, HIGH) //5v out to back of diode for caps
  }
  else if (activeMatrix == 2) {
    digitalWrite(9, HIGH) //5v out to back of diode for num
    digitalWrite(10, LOW) //GND this pin to pull voltage across capslk LED
  }
  else if (activeMatrix == 3) {
    digitalWrite(9, LOW) //GND this pin to pull voltage across numlk LED
    digitalWrite(10, LOW) //GND this pin to pull voltage across capslk LED
  }
  else {
  digitalWrite(9, HIGH) //5v out to back of diode for num
  digitalWrite(10, HIGH) //5v out to back of diode for caps
  }
}