#ifndef LedAlert_h
#define LedAlert_h
//#include "WConstants.h"
#include <Arduino.h>
//#include <ElementAbstract.h>
#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"

class LedAlertInterface: public BaseElement {
  public:    
    virtual void set(uint8_t red, uint8_t yellow, uint8_t green){}
    virtual void turnOnRed(){}
    virtual void turnOffRed(){}
    virtual void turnOnYellow(){}
    virtual void turnOffYellow(){}
    virtual void turnOnGreen(){}
    virtual void turnOffGreen(){}
};

class LedAlert: public LedAlertInterface {
  protected:
    uint8_t pinRed, pinYellow, pinGreen;

  public:
  LedAlert();
    LedAlert(uint8_t pinRed, uint8_t pinYellow, uint8_t pinGreen);
  void begin();
    void begin(uint8_t pinRed, uint8_t pinYellow, uint8_t pinGreen);
  void stop();

  uint8_t getCode();
  
    void set(uint8_t red, uint8_t yellow, uint8_t green);

    void turnOnRed();
    void turnOffRed();
    void turnOnYellow();
    void turnOffYellow();
    void turnOnGreen();
    void turnOffGreen();
  
  //JsonInterface
  void jsonProperties(String &value);
    void parseJson(String* json);
  //EEPROM
  uint8_t positions();
  void readFromEeprom(int pos);
  void saveInEeprom(int pos);
};

class LedRGBAlert: public LedAlertInterface {
  protected:
    int pinRed, pinGreen, pinBlue;

  public:
  LedRGBAlert();
    LedRGBAlert(uint8_t pinRed, uint8_t pinGreen, uint8_t pinBlue);
  void begin();
    void begin(uint8_t pinRed, uint8_t pinGreen, uint8_t pinBlue);
  void stop();

  uint8_t getCode();
  
    void set(uint8_t red, uint8_t green, uint8_t blue);

    void turnOnRed();
    void turnOffRed();
    void turnOnYellow();
    void turnOffYellow();
    void turnOnGreen();
    void turnOffGreen();
  
  //JsonInterface
  void jsonProperties(String &value);
    void parseJson(String* json);
  //EEPROM
  uint8_t positions();
  void readFromEeprom(int pos);
  void saveInEeprom(int pos);
};

#endif
