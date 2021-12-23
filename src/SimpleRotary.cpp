/*
===============================================================================================================
SimpleRotary.h
Library for using rotary encoders with an Arduino
Learn more at [https://github.com/mprograms/SimpleRotary]

Supports:

- Getting rotary encoder rotation direction.
- Getting rotary select button press.
- Getting rotary select button long press.
- Getting rotary select button press time.
- Basic debouncing of rotary / button.
- Basic error correcting of out-of-sync rotational readings.
- Use of both high and low triggers.

===============================================================================================================

v1.0 - June 1, 2019
Written by MPrograms 
Github: [https://github.com/mprograms/]

Release under the GNU General Public License v3
[https://www.gnu.org/licenses/gpl-3.0.en.html]

===============================================================================================================

*/

#include "Arduino.h"
#include "SimpleRotary.h"

SimpleRotary::SimpleRotary(byte pinA, byte pinB, byte pinS)
{
  _pinA = pinA;
  _pinB = pinB;
  _pinS = pinS;
  _currentTime = millis();
  _debounceRTime = _currentTime;
  _debounceSTime = _currentTime;
  _errorTime = _currentTime;
  
  _setInputPins();
}




/**
	SET TRIGGER
	Sets the trigger for rotations / button press either to high or low.
	If set to HIGH then the library will automaticall enable pull up resisters for the rotaty
	encoder's pins.
	
	@since v0.1;
**/
void SimpleRotary::setTrigger(byte i)
{
  _trigger = i;
  _setInputPins();
}


/**
	SET DEBOUNCE DELAY
	Sets the debounce delay time in milliseconds.
	
	This number shold be set as small as possible. Higher values can result in missing input pulses.
	Finding this value will vary from one rotary encoder to another and can be found simply with
	trial and error.
	
	You can turn this feature off by setting the delay value to 0.
	
	@since v0.1;
**/
void SimpleRotary::setDebounceDelay(int i)
{
  _debounceRDelay = i;
}


/**
	SET ERROR CORRECTION DELAY
	Sets the error correction delay delay time in milliseconds.
	
	Setting a higher number here will result in smoother direction values at the cost of being able to quickly change directions. For example, setting a value of 250ms will require a the user to pause 1/4 second between changing direction of the rotary encoder. It takes most users longer then 250ms to reorient their fingers to the new direction so in most cases this would be fine. If a user was to change direction of the rotary encoder faster then 1/4 of a second then the output value would be the previous direction. This number should be set to whatever delay works best for your application.
	
	You can turn this feature off by setting the delay value to 0.
	
	@since v0.1;
**/
void SimpleRotary::setErrorDelay(int i)
{
  _errorDelay = i;
}


/**
	GET ROTARY DIRECTION
	Gets the direction the rotary encoder is turned.
	
	0x00 = Not turned.
	0x01 = Clockwise;
	0x02 = Counter-Clockwise
	
	@since v0.1;
**/
byte SimpleRotary::rotate()
{
  byte _dir = 0x00;
  _updateTime();
  
  if( _currentTime >= ( _debounceRTime + _debounceRDelay ) ) {

    _statusA = ( digitalRead(_pinA) == _trigger ? true : false);
    _statusB = ( digitalRead(_pinB) == _trigger ? true : false);
	 
    if( !_statusA && _statusA_prev ){

      if ( _statusB != _statusA ) {
        _dir = 0x01;
      } else {
        _dir = 0x02;
      }

      if ( _currentTime < (_errorTime + _errorDelay) ){
        _dir = _errorLast;
      } else {
		_errorLast = _dir;
	  }

      _errorTime = _currentTime;
	  
    }
    _statusA_prev = _statusA;
    _debounceRTime = _currentTime;
  }

  return _dir;

}


/**
	GET BUTTON PUSH
	Gets the status of the pushbutton
	
	Returned values
	0x00 = Not turned.
	0x01 = Clockwise;
	0x02 = Counter-Clockwise
	
	@since v0.1;
	@return byte, value of turned knob.
**/
byte SimpleRotary::push(){
	_updateTime();
	byte val = 0x00;

	_statusS = ( digitalRead(_pinS) == _trigger ? true : false);

	if ( _currentTime >= _debounceSTime + _debounceSDelay){
		if ( !_statusS &&  _statusS_prev ) {
			val = 0x01;
			_pulse = false;
			_pushTime = _currentTime;
		}
		_statusS_prev = _statusS;
		_debounceSTime = _currentTime;
	}
	return val; 
}


/**
	GET LONG BUTTON PUSH TIME
	Gets the amount of time in milliseconds that the button is held down.
	
	@since v0.1;
	
	@return int, time in MS that the button has been held down.
**/
int SimpleRotary::pushTime(){
	unsigned int t = 0;
	byte s = push();
	if ( !_statusS &&  !_statusS_prev ) {
		t = _currentTime - _pushTime;
	}
	return t;
}


/**
	RESET PUSH TIMER
	Resets the push time back to the current time.
	
	Use this function if you want to create a long press that will reset itseld after (n) milliseconds thus causing a pulse to be created every time it resets. For example if you want to have the button do somthing each time it is held down for 1 second you would call:
	
	void loop(){
		int t = rotary.pushTime();
		if ( t > 1000 ) {
			rotary.resetPush();
			// do something really cool
		}
	}
	
	@since v0.1;
**/
void SimpleRotary::resetPush(){
	_updateTime();
	_pushTime = _currentTime;
}


/**
	GET LONG BUTTON PUSH
	Checks to see if the button has been held down for (n) milliseconds.
	
	If placed in a loop, this function will return a steady stream of 0's until the button has
	been held down long enough. Once the button has been held down (n)th milliseconds, a pulse of 1 will be produced. If the button is continued to be held down past this, only 0's will be produced.
	The button needs to be released and then pressed again in order to reset.
	
	Returned values
	0x00 = Button not pressed long enough
	0x01 = Button was pressed long enough.
	
	@param i, int, the number of milliseconds the button needs to be pressed in order
				   to be considered a long press.
	
	@since v0.1;
	
	@return byte
**/
byte SimpleRotary::pushLong(int i) {
	unsigned int time = pushTime();
	byte val = 0x00;
	
	if ( (_currentTime + time > _currentTime + i ) && !_pulse ) {
		val = 0x01;
		_pulse = true;
	}
	return val;
}


/**
	GET BUTTON PUSH TYPE
	Checks to see if the button push is a short or long push
	
	Note: Unlike push(), pushType() generates a return value for a short press only on button release. This allows us to track a long press without a false positive of a short press. It is for this reason that you should use this function if you are tying to check for a long or short press in the same function.

	Returned values
	0x00 = Button not pressed.
	0x01 = Button was pressed.
	0x00 = Button was pressed for n milliseconds.
	
	@param i, int, the number of milliseconds the button needs to be pressed in order
				   to be considered a long press.
	
	@since v1.1.0;
	
	@return byte
**/
byte SimpleRotary::pushType(int i){
	_updateTime();
	_statusS = ( digitalRead(_pinS) == _trigger ) ? true : false;
	byte val = 0x00;

	if ( _currentTime >= _debounceSTime + _debounceSDelay ){
	
		// Button has been pressed
		if ( !_statusS && _statusS_prev ) {
			_btnPressed = true;
			_pushTime = _currentTime;
		}
		
		// Button has been released
		if ( _statusS && !_statusS_prev  && _btnPressed ) {
			_btnPressed = false;
			val = 0x01;
		}
		// Button is being held
		if ( !_statusS && !_statusS_prev && _btnPressed) {
			if ( _currentTime > _pushTime + i ) {
				_btnPressed = false;
				val = 0x02;
			}
		}
		
		_statusS_prev = _statusS;
		_debounceSTime = _currentTime;
	}
	
	
	return val; 
}





/**
	SET INPUT PINS
	Sets the input pins and pinmode based on the defined pins and _trigger value.
	
	@see setTrigger();
	
	@since v0.2;
**/
void SimpleRotary::_setInputPins(){
	if ( _trigger == HIGH ) {
		pinMode(_pinA, INPUT_PULLUP);
		pinMode(_pinB, INPUT_PULLUP);
		pinMode(_pinS, INPUT_PULLUP);
	} else {
		pinMode(_pinA, INPUT);
		pinMode(_pinB, INPUT);
		pinMode(_pinS, INPUT);
	}
}


/**
	UPDATE THE TIME
	Updates the _currentTime value to the current time in milliseconds.
	
	@since v0.1;
**/
void SimpleRotary::_updateTime()
{
  _currentTime = millis();
}