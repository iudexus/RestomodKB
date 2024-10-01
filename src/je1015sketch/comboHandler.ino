//this function can handle ctl and alt combos - oneday, windows combos as well! 

void comboHandler(uint16_t triggerKey) {

  //combo handler listens for the next strike just like shift
  //this part will also need an alt-checker that sends windows L UI key if shift is pressed immediately after alt
  //there's a reference in shiftHandler directing to this function, but only passinf Lalt, which should cause an immediate shift detection after landing here and send the UI key reliably as intended. (lol)

  //there is a delay and combo handler blinks the scrlk LED twice rapidly, then sends the command
  //the combo handler will wait for a 3rd  key in combo by running a while loop with delay indicated in the LED - so nested while loops to store the pins and scan other collumns


  return;
}