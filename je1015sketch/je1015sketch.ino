#include <Keyboard.h>

//define variables
const int rows = 8;  //pins connected to on-board 1k pullup resistors
const int cols = 11; //cins connected to "coloms"/traces out to keys

//define the specific pins for each operationol set
const int rpins[rows] = {0,1,2,3,4,5,6,7};
const int cpins[cols] = {11,12,13,14,15,16,17,18,19,20,21};

//define macros as variables with hex values

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
*/

/*set variable type to uint16_t instead of char to hold values over 255
Collom 11 (the last one) doesnt have all 8 characters
Representing w/ '\0' 
Escape both \ and ' chars w/ a leading \ */
uint16_t keymap[rows][cols] = {
  {'C', KEY_PERIOD, KEY_0, KEY_PRINTSCREEN, '\\', '.', 'M', 'B', 'Z', KEY_F7, '\0'}, //0
  {'S', KEY_F12, KEY_1, '`', ';', 'K', 'H', 'F', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'X', KEYPAD_PLUS, KEY_5, KEY_RIGHT_SHIFT, '/', ',', 'N', 'V', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'D', KEY_3, KEY_2, KEY_RETURN, '\'', 'L', 'J', 'G', KEY_F10, 'A', KEY_F8}, //3
  {'2', KEY_F11, KEY_NUM_LOCK, '=', '0', '8', '6', '4', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'W', KEY_9, KEY_7, ']', 'P', 'I', 'Y', 'R', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'E', KEY_6, KEY_4, ' ', '[', 'O', 'U', 'T', 'Q', KEY_F4, '\0'}, //6
  {'3', KEYPAD_MINUS, KEY_8, KEY_BACKSPACE, '-', '9', '7', '5', '1', KEY_F2, '\0'} //7
};

void setup() {
    //set up rows as inputs with pull-up resistors
  for (int i = 0; i < rows; i++) {
    pinMode(rpins[i], INPUT_PULLUP);
  }
    //set up columns as outputs, and initialize them to HIGH
  for (int i = 0; i < cols; i++) {
    pinMode(cpins[i], OUTPUT);
    digitalWrite(cpins[i], HIGH); //default column state is HIGH
  }

  Keyboard.begin();

  //delay to allow programming
  delay(4000);
}

void loop() {
  for (int row = 0; row < rows; row++) {
    //pull row LOW to scan it
    pinMode(rpins[row], OUTPUT);
    digitalWrite(rpins[row], LOW);

    //read the state of each column
    for (int col = 0; col < cols; col++) {
      if (digitalRead(cpins[col]) == LOW) {
        //key pressed at row and col
        char key = keymap[row][col];
        Keyboard.write(key);  //send to PC
        delay(100);  //debounce to avoid repeat
      }
    }

    //reset row to HIGH and input
    digitalWrite(rpins[row], HIGH);
    pinMode(rpins[row], INPUT_PULLUP);
  }
}
