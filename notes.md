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
 - unmangle numpad mappings (I wrote down the numbers, not the functions)
 - add boolean state flag variable to track num and caps lock states
 - Add minor anti-ghosting features in software like ignoring impossible key combinations or capping the number of simultaneous keypresses and test. 
 - Use for a few days and reassess anti-ghosting to implement more if needed. 

One possible problem is the lack of diodes.  I hope the keys have them inside, but I doubt that, so I'll likely have to compensate for ghosting in software. 

Now, I used a spreadsheet as a sortable reference when building the matrix, but I labeled my row values according to the on-board pullup resister positions they're connected to, which for whatever reason, is out of sequential order on the pinout.  For simplicity, I broke each row out into its own sheet in the spreadsheet and labeled them 0-7 for consistency with the matrix in code.  I do also have a column missing 4 values, so these will be represented with \0. 

Keyboard.h is no longer sufficient, also had to add KeyboardLayout.h to includes

game explosion magic around noon tmrw