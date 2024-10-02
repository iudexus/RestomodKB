//this function can handle ctl and alt combos - oneday, windows combos as well! 

void comboHandler(const int col1, uint16_t key1) {
//this function can handle shift as a matrix midifier AND as a 2 key combo trigger
  bool sent = false; //set flag to indicate at what level cmd was sent -- optimize later 

  while (analogRead(col1) >= 990) { //get 2nd key
    
    //begin scanning keys again for SECOND key in combo
    for (int row = 0; row < numrows; row++) {
      digitalWrite(rowpins[row], HIGH); //set rowpin HIGH to apply voltage and scan row

      for (int col2 = 0; col2 < numcols; col2++) { //read the state of each column

        if (analogRead(colpins[col2]) >= 1002 && col2 != col1) { //capture active column (analog)
        
        uint16_t key2 = matrices[activeMatrix][row][col2]; //correspond where key is pressed at row and col2 in the active matrix

          if (key2 == KEY_LEFT_SHIFT) {
            guiBlaster(KEY_LEFT_GUI); //hand off to send/process Windows/Command key
            break;
          }
          while (analogRead(col2) >= 990) { //get 3rd key

            //begin scanning keys again for THIRD key in combo
            for (int row = 0; row < numrows; row++) {
            digitalWrite(rowpins[row], HIGH); //set rowpin HIGH to apply voltage and scan row

              for (int col3 = 0; col3 < numcols; col3++) { //read the state of each column

                if (analogRead(colpins[col3]) >= 1002 && col3 != col1 && col3 != col2) { //capture active column (analog)
                
                uint16_t key3 = matrices[activeMatrix][row][col3]; //correspond where key is pressed at row and col3 in the active matrix

                  if (key2 != '\0' && key3 != '\0') { //check for null character
                  Keyboard.press(key1);
                  Keyboard.press(key2);
                  Keyboard.write(key3);  //send keypress to PC
                  Keyboard.releaseAll();
                  sent = true; //set flag to track if 
                  delay(180);  //immediate, extended debounce to avoid repeat
                  }
                }
              }
            }
          }
          if (key2 != '\0' && sent != true) { //check for null character
            Keyboard.press(key1);
            Keyboard.write(key2);  //send keypress to PC
            Keyboard.releaseAll();
            delay(180);  //immediate, extended debounce to avoid repeat
          }
        }
      }
    }
  }
  //CLEANUP: set rowpins low using dumb logic -- optimize later
  for (int row = 0; row < numrows; row++) {
  digitalWrite(rowpins[row], LOW);
  sent = false;
  }
}