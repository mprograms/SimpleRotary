#include <SimpleRotary.h>
//#include "Keyboard.h"
//#include "Mouse.h"

const int CLK = 4;      // Definition der Pins. CLK an D6, DT an D5. 
const int DT = 3;
const int SW = 2;       // Der Switch wird mit Pin D2 Verbunden. ACHTUNG : Verwenden Sie einen interrupt-Pin!
long Position = 0;  // Definition der "alten" Position (Diese fiktive alte Position wird benötigt, damit die aktuelle Position später im seriellen Monitor nur dann angezeigt wird, wenn wir den Rotary Head bewegen)
long PositionP = 0;  // Definition der "alten" Position (Diese fiktive alte Position wird benötigt, damit die aktuelle Position später im seriellen Monitor nur dann angezeigt wird, wenn wir den Rotary Head bewegen)

unsigned long _currentTime;
unsigned long _endrotateTime;

// LED Outputs
#define ledCW 5
#define ledCCW 6
#define ledBUT 7

SimpleRotary rotary(4,3,2);  // An dieser Stelle wird ein neues Encoder Projekt erstellt. Dabei wird die Verbindung über die zuvor definierten Varibalen (DT und CLK) hergestellt.


void setup()   // Beginn des Setups

{
  Serial.begin(9600); 
}

void loop() {

  _currentTime = millis();
  
  byte i;
  byte j;

  // 0 = not turning, 1 = CW, 2 = CCW
  i = rotary.rotate();
  // 0 = not pushed, 1 = pushed, 2 = long pushed
  j = rotary.pushType(10000); // number of milliseconds button has to be pushed for it to be considered a long push.

  if ( i == 1 && j != 4) {
    Position = Position + 1;
    Serial.print("CW ");
    Serial.println(Position);
    digitalWrite(ledCW, HIGH);
    digitalWrite(ledCCW, LOW);
    digitalWrite(ledBUT, LOW);
  }

  if ( i == 2 && j != 4) {
    Position = Position -1;
    Serial.print("CCW ");
    Serial.println(Position);
    digitalWrite(ledCW, LOW);
    digitalWrite(ledCCW, HIGH);
    digitalWrite(ledBUT, LOW);
  }

  
  if ( i == 1 && j == 4) {
    PositionP = PositionP + 1;
    _endrotateTime = _currentTime;
    rotary.resetPush();
    Serial.print("BCW ");
    Serial.println(PositionP);
    digitalWrite(ledCW, HIGH);
    digitalWrite(ledCCW, HIGH);
    digitalWrite(ledBUT, LOW);
  }

  if ( i == 2 && j == 4) {
    PositionP = PositionP -1;
    _endrotateTime = _currentTime;
    rotary.resetPush();
    Serial.print("BCCW ");
    Serial.println(PositionP);
    digitalWrite(ledCW, LOW);
    digitalWrite(ledCCW, HIGH);
    digitalWrite(ledBUT, HIGH);
  }

  if ( j == 1  && _currentTime >= _endrotateTime + 1000) {
    Serial.println("Pushed");
    digitalWrite(ledCW, LOW);
    digitalWrite(ledCCW, LOW);
    digitalWrite(ledBUT, HIGH);

  }
  
  if ( j == 2  && _currentTime >= _endrotateTime + 1000) {
    Serial.println("Long Pushed");
    digitalWrite(ledCW, HIGH);
    digitalWrite(ledCCW, HIGH);
    digitalWrite(ledBUT, HIGH);
  }
}
