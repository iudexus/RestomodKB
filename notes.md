Parts: teensy 2.0, usb B mini cord, je1015 keyboard w/ alpine blue switches c1987 0.1uf polyester cap

Steps:
Created spreadsheet to help make sense of the key matrix.

Installed Arduino IDE v2.3.2 to be compatible with teensy and added teensyduino.

Loaded up a blink test demo from Arduino IDE examples and adjusted teensy default 1s blink to 10s interval to test functionality. 

Initial planning and research to determine following scope. 

Scope: 
 - program 84 keys in 8x11 matrix - done
 - add code to check for correct key press and skip to sending output. 
 - program logic to represent caps and num lock via LED 
 - connect RST pin to scrlk LED, then to tx/at switch pins 4&5, then to .1uf poly cap, and finally to gnd. 
 - connect as much as possible with jumpers and pre-solder everything to existing IC1286 holder for easy hookup
 -  add code to handle special characters with shift combos
 - write a statement to capture Lalt+Lshift combo and send Lgui instead since the keyboard is so old it doesn't have a Windows key
 - unmangle numpad mappings (I wrote down the numbers, not the functions) [Keyboard Modifiers and Special Keys - Arduino Reference](https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/)
 - add boolean state flag variable to track num and caps lock states
 - Add minor anti-ghosting features in software like ignoring impossible key combinations or capping the number of simultaneous keypresses and test. 
 - Use for a few days and reassess anti-ghosting to implement more if needed. 

One possible problem is the lack of diodes.  I hope the keys have them inside, but I doubt that, so I'll likely have to compensate for ghosting in software. 

Now, I used a spreadsheet as a sortable reference when building the matrix, but I labeled my row values according to the on-board pullup resister positions they're connected to, which for whatever reason, is out of sequential order on the pinout.  For simplicity, I broke each row out into its own sheet in the spreadsheet and labeled them 0-7 for consistency with the matrix in code.  I do also have a column missing 4 values, so these will be represented with \0. 

Keyboard.h is no longer sufficient, also had to add KeyboardLayout.h to includes

game explosion magic around noon tmrw

[How to emulate a keyboard or mouse with Arduino (luisllamas.es)](https://www.luisllamas.es/en/emulate-keyboard-mouse-arduino/)

For testing, pin names to Teensy may not match assignments in code. See below:
![[teensy20_card2b_rev5.png]]


Here is the original location of the 2 replaced keys: 
![[Screenshot 2024-09-22 000656.png]]


Had trouble calling the keyboard library due to having serial selected in the tools > USB Type menu

Fails to compile with the macros from arduino's website, but the errors are recommending replacements now.  Updated macros to the recommended format. 

Had to set keymap variable type to uint16_t instead of char to hold values over 255 for hex

Now to add some debugging, vagely by: include serial lib, init serial at 9600 baud, capture the keystroke in a variable and instead of sending it via keyboard write, sent it as serial write

So the keyboard has a variety of resistances keeping pin states floating.  I seem to be able to hold the rows reliably high (best done with pull up resistors), but columns go randomly low.  I'll have to find a way to analog read the column when it falls under a certain threshold of resistance (dead short basically), then find a way to identify which row it's on. 

Finally!!  I was able to get the main code working with analog signal on columns and digital on rows!  Keystrike sensing is dialed in for the letter S lol I dont know how it will be for everything else, hopefully the same parameters.  Serial console reports in the 700-960 range when open and 100-1024 when the key is depressed.  I see no reason that shouldn't be the case throughout, but with the way this thing is going, who knows. 

I just need to clean up my code and document.  I have a function at the end of the main project that I need to move back to test because the arithmetic simply takes too long.  If I need to do and keysignal-cleanup-jutsu, I will, but this system seems to work pretty well at a 160ms delay.

Next steps SOLDERING:
- need to retest kb function with switch in the xt position.
- Identify whether circuit is possible for trace re-use since voltage was always applied to LED circuit - one possible fix here is to simply hardwire in place of the resistors and wire to gnd, then place resistors on the jumpers in for each LED and reverse polarity
- remove jumper for voltage line in from PC header next to switch and retest
- Add momentary cap replacing jumper in SCRLK line to switch
- remove extra/extraneous hardware - or not. At least disconnect logic diode D1. 

Hybrid note: the LEDs are set up for control via voltage sinking on a shared power rail.  Supply 5v to the rail and use 2 pins to sink for CPS and NUM.  look into how reset could be shorted on at/tx switching using momentary circuit still to allow light to light on at setting.  - there must be some old logic circuit here to use, probably out to USB header --there are 2 pins in IC header connecting directly to switch outputs like I suspected.  Must be TX and RX.  WHerever that lands, I can install my reset circuit and 50% chance it'll work with the scrlk light

Next steps PROGRAMMING:
 - Identify which variables types need to be matching if any to handle shift case
 - identify how to handle shift case since letters are all being sent as capitals
	 - Perhaps duplicate matrix with caps only, flag shift case, and set kb to never pass a null character, which I already need to do
- set kb to never pass a null variable. 
- Create caps case and numlk case and add similar filtering to above
- perhaps optimize normal keystrokes by detecting global special case bool - I dont type that fast on the numpad for the extra processing time to matter for checking all cases, but I might in normal typeing (the break character will be useful to check spcl case bool flag, then exit on FALSE)
- As above, I need to figure out the macros for the normal functioning numpad map and update the matrix.  I think it's best to create a "special case matrix" variable that coordinates can be passed into on spcl case conditions
	- check here for current definitions of macros [Keyboard/src/Keyboard.h at master · arduino-libraries/Keyboard (github.com)](https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h) and here for decimal values [Keyboard Modifiers and Special Keys - Arduino Reference](https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/)

Lorem ipsum dolor sit amet!

changing direction of the shift/caps/numlk handling: instead of handling this in nested, dependent if statements and while loops, I've created an array to map the state of the kayboard to the appropriate matrix.  This way the loop will be far smaller, cleaner, and faster. 

helpful reference.  Use goto to skip checking if no change to active matrix!! 
cache the last used matrix in a static variable and have a single checking if statement that calls the state changer as a function: "activeMatrixUpdater" which can switch the matrix

Here's an Idea!  Since I dont have a windows key, have ALT+shift = r_gui, then have a timer where the scrlk key blinks a few seconds awaiting key combo else sends r_gui keypress

The next big things are the above key combo handler, LED logic with voltage sinking (1 pin pullup, 3 pins high), and actually writing out the key matrices since that'll be time consuming.  Then wiring! 

LEDS pins 9, 10 and 24 - 5v from 23

keyboard lib troubleshooting: [Teensyduino: Using USB Keyboard with Teensy on the Arduino IDE (pjrc.com)](https://www.pjrc.com/teensy/td_keyboard.html#:~:text=There%20are%20two%20ways%20you%20can%20make%20your)