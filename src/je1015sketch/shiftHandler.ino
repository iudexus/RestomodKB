//this function can handle shift as a matrix midifier AND as a 2 key combo trigger

void shiftHandler(const int shiftPin, uint16_t shiftKey) {

  //activeMatrix |= shift //use bitwise 'or' statement to turn shift on

  while (analogRead(shiftPin) >= 990) {
    //begin scanning keys

    for (int row = 0; row < numrows; row++) {
      //set rowpin HIGH to apply voltage and scan row
      digitalWrite(rowpins[row], HIGH);

      //read the state of each column
      for (int col = 0; col < numcols; col++) {
      
        //capture active column (analog)
        if (analogRead(colpins[col]) >= 1002 && col != shiftPin) { 
        
        //correspond where key is pressed at row and col in the active matrix
        uint16_t newKey = matrices[activeMatrix][row][col]; //variable type set to uint16_t to hold macros - treat as char
          if (newKey != '\0') { //check for null character
            Keyboard.press(shiftKey);
            Keyboard.write(newKey);  //send keypress to PC
            Keyboard.releaseAll();
            delay(180);  //immediate, extended debounce to avoid repeat
            //activeMatrix &= ~shift //use bitwise 'and not' statement to turn shift off for cleanup
            return; //start from the top
          }
        }
      }
    }
  }
}