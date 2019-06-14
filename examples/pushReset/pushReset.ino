/*
===============================================================================================================
SimpleRotary.h Library Button Press Time Reset Example Sketch
Learn more at [https://github.com/mprograms/SimpleRotary]

This example shows how to reset the time the button was pressed.

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
  
  int i;
  i = rotary.pushTime();

  // If button is pressed for more then 2 seconds, then reset the amount of time it was pressed.
  // This can be used to build a function that will repeat every 2 seconds.
  
  if ( i > 2000 ) {
    rotary.resetPush();
  }
  
  Serial.println(i);
}
