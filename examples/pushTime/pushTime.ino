/*
===============================================================================================================
SimpleRotary.h Library Button Press Time Example Sketch
Learn more at [https://github.com/mprograms/SimpleRotary]

This example shows how to get amount of time the button was pressed.

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
  Serial.println(i);
}
