#ifndef SimpleRotary_h
#define SimpleRotary_h

#include "Arduino.h"

class SimpleRotary
{
  public:
    SimpleRotary(byte pinA, byte pinB, byte pinS);
    byte rotate();
    byte push();
	int pushTime();
	byte pushLong(int i);
	byte pushType(int i);
    void setTrigger(byte i);
    void setDebounceDelay(int i);
    void setErrorDelay(int i);
	void resetPush();
	
  private:
    byte _trigger = HIGH;
    byte _pinA;
    byte _pinB;
    byte _pinS;
    unsigned long _currentTime;
    unsigned long _debounceRTime;
    unsigned long _debounceSTime;
	unsigned long _pushTime;
    unsigned int _debounceRDelay = 2;
    unsigned int _debounceSDelay = 200;
	bool _pulse = false;
	bool _pushStatus = false;
	bool _btnPressed = false;
	unsigned long _errorTime;
    unsigned int _errorDelay = 100;	
    byte _errorLast = 0;
    bool _statusA = false;
    bool _statusB = false;
    bool _statusS = false;
    bool _statusA_prev = false;
    bool _statusS_prev = false;
    void _updateTime();
	void _setInputPins();
};

#endif