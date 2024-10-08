//this code to replace/compensate for lack of gui key
void guiBlaster(uint16_t guiKey) {
  //blinky lights and other stuff
  //do while for this part to force Lgui
  bool sent = false; //set flag to indicate if command was sent early -- optimize later --- this can prob be eliminated in favor of break for this one.  Test and optimize
  int loopCounter = 0;

  while (loopCounter <= 20) { //get 2nd key

    loopCounter++; //increase counter to number loops

    digitalWrite(24, !digitalRead(24));  //toggle scrlk light every 1/10th of a scond or so

    delay(100);

    //begin scanning keys again for SECOND key in combo
    for (int row = 0; row < numrows; row++) {
      digitalWrite(rowpins[row], HIGH); //set rowpin HIGH to apply voltage and scan row

      for (int col2 = 0; col2 < numcols; col2++) { //read the state of each column

        if (analogRead(colpins[col2]) >= 1002) { //capture active column (analog)
        
        uint16_t key2 = matrices[activeMatrix][row][col2]; //correspond where key is pressed at row and col2 in the active matrix

          while (analogRead(col2) >= 990) { //get 3rd key

            digitalWrite(24, LOW); //set scrlk to on to indicate the combo is reading

            //begin scanning keys again for THIRD key in combo
            for (int row = 0; row < numrows; row++) {
            digitalWrite(rowpins[row], HIGH); //set rowpin HIGH to apply voltage and scan row

              for (int col3 = 0; col3 < numcols; col3++) { //read the state of each column

                if (analogRead(colpins[col3]) >= 1002 && col3 != col2) { //capture active column (analog)
                
                uint16_t key3 = matrices[activeMatrix][row][col3]; //correspond where key is pressed at row and col3 in the active matrix

                  if (key2 != '\0' && key3 != '\0') { //check for null character
                  Keyboard.press(guiKey);
                  Keyboard.press(key2);
                  Keyboard.write(key3);  //send keypress to PC
                  Keyboard.releaseAll();
                  sent = true; //set flag to track if 
                  delay(180);  //extended debounce to avoid repeat
                  break; //while loop and proceed to cleanup
                  }
                }
              }
            }
          }
          if (key2 != '\0' && sent != true) { //check for null character
            Keyboard.press(guiKey);
            Keyboard.write(key2);  //send keypress to PC
            Keyboard.releaseAll();
            delay(180);  //extended debounce to avoid repeat
          }
          else if (sent == true) {
            break; //exit main while loop and proceed to cleanup
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
  return;
  digitalWrite(24, HIGH); //5v out to back of diode again for cleanup
}
