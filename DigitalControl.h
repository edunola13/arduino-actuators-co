#ifndef DigitalControl_h
#define DigitalControl_h
//#include "WConstants.h"
#include <Arduino.h>
//#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
#include "../arduino-basic-oo/ElementAbstract.h"

class DigitalControl: public BaseElement {
  protected:
    uint8_t pinD, state;    
  
  public:    
	//1= Digital, 2= PWM
    uint8_t type= 1;
  
    DigitalControl();
    DigitalControl(uint8_t pinD, uint8_t type);
    void begin();
    void begin(uint8_t pinD, uint8_t type);
    void stop();
  
	uint8_t getCode();
  
    void set(uint8_t value);

    uint8_t getPinD();
    void setPinD(uint8_t pinD);
    uint8_t getState();

    //JsonInterface
    void jsonProperties(String &value);
    void parseJson(String* json);
	  //EEPROM
	  uint8_t positions();
	  void readFromEeprom(int pos);
	  void saveInEeprom(int pos);
};

#endif
