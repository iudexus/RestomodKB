#include <Keyboard.h>
#include <KeyboardLayout.h>

//define variables
const int rows = 8;  //pins connected to on-board 1k pullup resistors
const int cols = 11; //cins connected to "coloms"/traces out to keys

//define the specific pins for each operationol set
const int rpins[rows] = {0,1,2,3,4,5,6,7};
const int cpins[cols] = {11,12,13,14,15,16,17,18,19,20,21};

/*collom 11 (the last one) doesnt have all 8 characters, therefore representing w/ '\0' 
escape both \ and ' chars w/ a leading \ */

char keymap[rows][cols] = {
  {'C', KEY_KP_DOT, KEY_KP_0, KEY_PRINT_SCREEN, '\\', '.', 'M', 'B', 'Z', KEY_F7, '\0'}, //0
  {'S', KEY_F12, KEY_KP_1, '`', ';', 'K', 'H', 'F', KEY_LEFT_ALT, KEY_TAB, KEY_F5}, //1
  {'X', KEY_KP_PLUS, KEY_KP_5, KEY_RIGHT_SHIFT, '/', ',', 'N', 'V', KEY_CAPS_LOCK, KEY_LEFT_SHIFT, KEY_F6}, //2
  {'D', KEY_KP_3, KEY_KP_2, KEY_RETURN, '\'', 'L', 'J', 'G', KEY_F10, 'A', KEY_F8}, //3
  {'2', KEY_F11, KEY_NUM_LOCK, '=', '0', '8', '6', '4', KEY_F9, KEY_ESC, KEY_F1}, //4
  {'W', KEY_KP_9, KEY_KP_7, ']', 'P', 'I', 'Y', 'R', KEY_LEFT_CTRL, KEY_F3, '\0'}, //5
  {'E', KEY_KP_6, KEY_KP_4, ' ', '[', 'O', 'U', 'T', 'Q', KEY_F4, '\0'}, //6
  {'3', KEY_KP_MINUS, KEY_KP_8, KEY_BACKSPACE, '-', '9', '7', '5', '1', KEY_F2, '\0'} //7
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
  for (int row = 0; row < numRows; row++) {
    //pull row LOW to scan it
    pinMode(rpins[row], OUTPUT);
    digitalWrite(rpins[row], LOW);

    //read the state of each column
    for (int col = 0; col < numCols; col++) {
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
