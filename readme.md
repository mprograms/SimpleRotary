# SimpleRotary Arduino Library

by [MPrograms](https://github.com/mprograms/)
| 
[Github Project Page](https://github.com/mprograms/SimpleRotary/)

Released under the GNU Public License v3 (GPLv3)

---
## Overview
SimpleRotary is a Arduino library for using basic bare-bones type rotary encoders. Usually these are the cheap ones without the breakout board.

Supports:

- Getting rotary encoder rotation direction.
- Getting rotary select button press.
- Getting rotary select button long press.
- Getting rotary select button press time.
- Basic debouncing of rotary / button.
- Basic error correcting of out-of-sync rotational readings.
- Use of both high and low triggers.



## Rotary Hookup Reference

Rotary encoder pinouts can differ depending on the manufacturer. You will need to test your rotary encoder to determine the type you have. The reference below also assumes that you are using the default trigger (HIGH).

```
BTN 1 O ---- O PIN 7
BTN 2 O ---- O GND
    A O ---- O PIN 6
    B O ---- O PIN 5
  COM O ---- O GND

```

---

## Arduino Code

### Getting Started
To begin, include the SimpleRotary Library at the top of your sketch.

```
#include <SimpleRotary.h>
SimpleRotary rotary;
```


### Getting Values

SimpleRotary Library makes it easy to get input. Call any of the following within the loop.

#### Getting Up / Down Rotation
To get the up / down rotation, simply call `rotate()`.

```
void loop(){
	byte i;
	i = rotary.rotate();
	
	// CW
	if ( i == 1 ) {
		Serial.println("CW");
		// Do something cool
	}
	
	// CCW
	if ( i == 2 ) {
		Serial.println("CCW");
		// Do something cool
	}
}
```

This function will return a single byte of three possible values:

- 0 - Rotary has not turned.
- 1 - Rotary has turned CW.
- 2 - Rotary has turned CCW.


#### Getting Button Push
To tell whether or not the button has been pressed simply call `push();`.

```
void loop(){
	byte i = rotary.push();
	
	if ( i == 1 ){
		Serial.println("Pushed");
		// Do something cool
	}
}
```

This function will return a single byte of two possible values:

- 0 - Button was not pushed.
- 1 - Button was pushed.

#### Getting Button Long Push
To tell whether or not the button has been pressed for (n) milliseconds, you can call `pushLong(n)`. You can set the value of n to any integer. As soon as n milliseconds has elapsed with the button pressed, a single byte 1 will be returned.

```
void loop(){
   byte i = rotary.pushLong(1000);
   
   if ( i == 1 ){
		Serial.println("Long Pushed");
		// Do something cool
	}
}
```

This function will return a single byte of two possible values:

- 0 - Button was not long pushed.
- 1 - Button was long pushed.

Note: This function will only return a single 1 pulse for each button press. Holding the button for (n * 2) will not result in a second 1 being returned. To return pulsed output, please see "Resetting Long Push Time".


#### Getting Button Long Push Time
To get the number of milliseconds the button has been pushed you can call `pushTime`.

```
void loop(){
	int i = rotary.pushTime();
	
	Serial.print("Pressed: ");
	Serial.print(i);
	Serial.println();
}
```

#### Resetting Long Push Time
When you release the button the `pushTime()` output value will return to 0. There may be times, however, that you want to reset the time without releasing the button. By doing this, you can achieve pulsed output from a single long press.

```
void loop(){
	int i = rotary.pushTime();
	
	if ( i > 1000 ) {
		rotary.resetPush();
		Serial.println("Reset!");
		// Do something cool
	}
	
	Serial.print("Pressed: ");
	Serial.print(i);
	Serial.println();
}
```

#### Getting Short & Long Button Pushes Together
There are times where you may want to use both short and long presses together, such as in a menu system. In these cases use `pushType(n);` Where n is the number of milliseconds you want a long press to be.

Calling `push();` and `pushLong(n);` will not work in these cases because `push();` generates a return output on button press, resulting in invalid results. Using `pushType(n);` generates return output for a short press only on button release.

This function will return a single byte of three possible values:

- 0 - Button was not pushed.
- 1 - Button was short pushed.
- 2 - Button was long pushed.

```
void loop(){
	byte t = rotary.pushType();
	// Short Push
	if ( t == 1 ) {
		// Do something cool.
	}
	//Long push
	if ( t == 2 ) {
		// Do something cool.
	}
}
```

---

## Example Sketch

### Example Code

The below sketch will create a counter and manipulate it by means of the rotary encoder.

- CW Rotation: Add 1 to the counter.
- CCW Rotation: Subtract 1 from the counter.
- Button Press: Add 100 (CW) or subtract 100 (CCW) to the counter based on the last direction of the rotary encoder.
- Long Button Press: Reset the counter back to 0.

```

#include <SimpleRotary.h>

// Pin A, Pin B, Button Pin
SimpleRotary rotary(6,5,7);

int ctr = 0;
byte lastDir = 0;

void setup() {
  Serial.begin(9600);
}
 
void loop(){
  int rDir = rotary.rotate();
  int rBtn = rotary.push();
  int rLBtn = rotary.pushLong(1000);

  // Check direction
  if ( rDir == 1  ) {
      // CW
      ctr++;
      lastDir = rDir;
  }
  if ( rDir == 2 ) {
      // CCW
      ctr--;
      lastDir = rDir;
  }

  if ( rBtn == 1 ) {
    switch (lastDir) {
      case 1:
        ctr += 100;
        break;
      case 2:
        ctr -= 100;
        break;
    }
  }

  if ( rLBtn == 1 ) {
    ctr = 0;
  }

  // Only Serial.print when there is user input.
  // Constant serial printing without delay can introduce lag where rotary encoder rotation pulses can be missed.
  if ( rDir == 1 || rDir == 2 || rBtn == 1 || rLBtn == 1 ){
    Serial.print("Counter = ");
    Serial.print(ctr);
    Serial.println();
  }
}
```


---

## Options & Settings


### Trigger States
SimpleRotary allows for triggering either by HIGH or LOW states. By default HIGH is selected. You can change this setting to LOW by calling `setTrigger(LOW);` in the `setup()` function like so:

```
void setup(){
	rotary.setTrigger(LOW);
}
```

### Debounceing
SimpleRotary includes a basic debouncing function. By default, the debounce delay is set to 2ms. You can increase this by calling `setDebounceDelay(n);` where n is the number of milliseconds to delay. Higher values will improve debouncing but may also result in missed pulses from the rotary encoder. Place this function in your `setup()` function. You can turn this feature off by setting n to 0.

```
void setup(){
	// Set to 5 milliseconds
	rotary.setDebounceDelay(5);
}
```

### Error Correction
SimpleRotary includes a basic error correction function that will help to remove erroneous pulses when rotating at higher speeds. By default, this value is set to 200ms. You can change this value by calling `setErrorDelay(n);` where n is the number of milliseconds to delay. Setting n to a higher number will result in smoother direction values being returned at the cost of being able to quickly change directions.

For example, setting a value of 250ms will require the user to pause 1/4 second between changing direction of the rotary encoder. It takes most users longer then 250ms to reorient their fingers to the new direction so in most cases this would be fine. If a user was to change direction of the rotary encoder faster then 1/4 of a second then the output value would be the previous direction. This number should be set to whatever delay works best for your application. Place this function in your `setup()` function. You can turn this feature off by setting n to 0.


```
void setup(){
	// Set to 5 milliseconds
	rotary.setErrorDelay(5);
}
```

---

## Troubleshooting

### Rotation Gives Backwards Reading or Only Goes CW / CCW

- Check to see if your rotary encoder pins are not reversed either in actual wiring or defined pins in the `rotary(A, B, Button);` initialization.
- Check to make sure you are using the correct pinout of your rotary encoder. Different manufacturers use different pinouts.

### Unreliable Rotation Readings While Using Serial.print()

Calling Serial.print() too quickly can cause pulses from the rotary encoder to be missed. If this happens you will see output bouncing between CW and CCW when turning in a given direction.

- Limit your calls to Serial.print() on rotation / button press.
- Place a delay between Serial.print()'s to allow the pulses to keep up.

### Unreliable Rotation Readings While Using LCD

Refreshing or rewriting your LCD too often can result in missed pulses from the rotary encoder when it is turning at higher speeds. This is caused by the extra time it takes the LCD to completely rewrite. There are a couple options to fixing this issue.

- Don't rewrite the entire lcd screen each time there is a pulse from the rotary encoder. Only update the part of the screen that is required.
- Use a short delay to update the lcd after (n) milliseconds.