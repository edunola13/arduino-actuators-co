#ifndef Relay_h
#define Relay_h
//#include "WConstants.h"
#include <Arduino.h>
#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"

class Relay: public BaseElement{
  protected:
    uint8_t pin;
    uint8_t state= LOW;	

  public:
    Relay();
    Relay(uint8_t pin);
	void begin();
	void begin(uint8_t pin);
	void stop();
		
	uint8_t getCode();
		
    void commonlyOpen();
    void commonlyClose();
    void changeStatus();
	
    uint8_t getPin();
    void setPin(uint8_t pin);
	uint8_t getState();	
	
	//JsonInterface
	void jsonProperties(String &value);
    void parseJson(String* json);
	//EEPROM
	uint8_t positions();
	void readFromEeprom(int pos);
	void saveInEeprom(int pos);
};

class ControlRelayInterval: public BaseElement{
  protected:
	uint8_t relayId, relayCode;
    Relay *relay= NULL;
	uint8_t sensorId, sensorCode;
    SensorInterface *sensor= NULL;	
	unsigned long lastTime= 0;

  public:
	int max, min;
	uint8_t typeLevel= 0;
    bool invert= false;
	uint8_t active= 0;
	unsigned long time= 2000;
  
    ControlRelayInterval();
    ControlRelayInterval(Relay *relay, SensorInterface *sensor, bool invert);
    void begin();
    void begin(Relay *relay, SensorInterface *sensor, bool invert);
	void stop();

	uint8_t getCode();
	
    bool updateStatus();
	
	uint8_t getRelayId();
	uint8_t getRelayCode();
	uint8_t getSensorId();
	uint8_t getSensorCode();
	void setRelay(Relay *relay, uint8_t id);
	void setSensor(SensorInterface *sensor, uint8_t id);	
	//JsonInterface
	void jsonProperties(String &value);
    void parseJson(String* json);
	//EEPROM
	uint8_t positions();
	void readFromEeprom(int pos);
	void saveInEeprom(int pos);
};

#endif
