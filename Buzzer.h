#ifndef Buzzer_h
#define Buzzer_h
//#include "WConstants.h"
#include <Arduino.h>
#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"

class Buzzer: public BaseElement {
  protected:    
    uint8_t pin;	

  public:
    //1= with function tone(), 2= Manual
    uint8_t type= 1;
  
	Buzzer();
    Buzzer(uint8_t pin, uint8_t type= 1);
	void begin();
	void begin(uint8_t pin, uint8_t type= 1);
	void stop();
    
	uint8_t getCode();
	
    void soundTone(uint16_t ton);
    void soundTone(uint16_t ton, uint16_t duration);
    void turnOut();
    	
    uint8_t getPin();
    void setPin(uint8_t pin);
	
	//JsonInterface
	void jsonProperties(String &value);
    void parseJson(String* json);
	//EEPROM
	uint8_t positions();
	void readFromEeprom(int pos);
	void saveInEeprom(int pos);
};

class ControlBuzzerInterval: public BaseElement{
  protected:
	uint8_t buzzerId, buzzerCode;	
    Buzzer *buzzer= NULL;
	uint8_t sensorId, sensorCode;
    SensorInterface *sensor= NULL;    
	bool active= false;

  public:
	uint16_t tone;
	int max, min;
	uint8_t typeLevel= 0;
	uint16_t duration= 100;
	bool repeat= true;
  
    ControlBuzzerInterval();
    ControlBuzzerInterval(Buzzer *buzzer, SensorInterface *sensor, uint16_t tone);
    void begin();
    void begin(Buzzer *buzzer, SensorInterface *sensor, uint16_t tone);
	void stop();
	
	uint8_t getCode();
	
    bool updateStatus();
	
	uint8_t getBuzzerId();
	uint8_t getBuzzerCode();
	uint8_t getSensorId();
	uint8_t getSensorCode();
	void setBuzzer(Buzzer *buzzer, uint8_t id);
	void setSensor(SensorInterface *sensor, uint8_t id);	
	bool getActive();
	void setActive(bool active);
	//JsonInterface
	void jsonProperties(String &value);
    void parseJson(String* json);
	//EEPROM
	uint8_t positions();
	void readFromEeprom(int pos);
	void saveInEeprom(int pos);
};

#endif
