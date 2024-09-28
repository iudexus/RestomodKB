//this function toggles num and cap states
void numCapsChanger(uint16_t inKey) {
  if (inKey == KEY_CAPS_LOCK) {
    modifierState ^= capslk;
  }
  else if (inKey == KEY_NUM_LOCK) {
    modifierState ^= numlk;
  }
}