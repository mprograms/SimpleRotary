/*
===============================================================================================================
SimpleRotary.h Library Options Example Sketch
Learn more at [https://github.com/mprograms/SimpleRotary]

This example shows how to set various options.

===============================================================================================================
Release under the GNU General Public License v3
[https://www.gnu.org/licenses/gpl-3.0.en.html]
===============================================================================================================
*/
#include <SimpleRotary.h>

// Pin A, Pin B, Button Pin
SimpleRotary rotary(6,5,7);

  

void setup() {

  // Set the trigger to be either a HIGH or LOW pin (Default: HIGH)
  // Note this sets all three pins to use the same state.
  rotary.setTrigger(HIGH)

  // Set the debounce delay in ms  (Default: 2)
  rotary.setDebounceDelay(5)

  // Set the error correction delay in ms  (Default: 200)
  rotary.setErrorDelay(250)
}

void loop() {
  byte i;
  i = rotary.rotate();
  Serial.println(i);
}
