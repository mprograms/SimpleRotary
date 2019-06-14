/*
===============================================================================================================
SimpleRotary.h Library Button Press Example Sketch
Learn more at [https://github.com/mprograms/SimpleRotary]

This example shows how to get the button press.

===============================================================================================================
Release under the GNU General Public License v3
[https://www.gnu.org/licenses/gpl-3.0.en.html]
===============================================================================================================
*/
#include <SimpleRotary.h>

// Pin A, Pin B, Button Pin
SimpleRotary rotary(6,5,7);

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  byte i;

  // 0 = not pushed, 1 = pushed
  i = rotary.push();

  if ( i == 1 ) {
    Serial.println("Pushed");
  }
}
