//this function toggles num and cap states
void numCapsChanger(uint16_t inKey) {
  if (inKey == KEY_CAPS_LOCK) {
    activeMatrix ^= capslk;
  }
  else if (inKey == KEY_NUM_LOCK) {
    activeMatrix ^= numlk;
  }
}