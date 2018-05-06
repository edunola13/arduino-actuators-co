//#include "WProgram.h"
#include <Arduino.h>
//#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
#include "../arduino-basic-oo/ElementAbstract.h"
#include "DigitalControl.h"

DigitalControl::DigitalControl(){}
DigitalControl::DigitalControl(uint8_t pinD, uint8_t type){
  this->begin(pinD, type);
}
void DigitalControl::begin(uint8_t pinD, uint8_t type) {  
  this->pinD= pinD;
  this->type= type;
  this->begin();
}
void DigitalControl::begin() {
  pinMode(this->pinD, OUTPUT);  
  this->started= true;
  this->set(0);
}
void DigitalControl::stop() {
  this->started= false;
}

uint8_t DigitalControl::getCode(){
	return 105;
}

void DigitalControl::set(uint8_t value){
  if(this->type == 1){
	  digitalWrite(this->pinD, value);
  }else{
    analogWrite(this->pinD, value);
  }
  this->state=value;
}

uint8_t DigitalControl::getPinD(){
  return this->pinD;
}
void DigitalControl::setPinD(uint8_t pinD){
  this->pinD= pinD;
  this->begin();
}
uint8_t DigitalControl::getState(){
  return this->state;
}

//JSON
void DigitalControl::jsonProperties(String &value){
  value+= this->propertyToJson("st", String(this->started), false, false);
  value+= this->propertyToJson("pinD", String(this->pinD), false, false);
  value+= this->propertyToJson("ty", String(this->type), false, false);
  value+= this->propertyToJson("sta", String(this->state), false, true);
}
void DigitalControl::parseJson(String* json){
  String value= parseProperty(json, "pinD");
  if(isNotNull(value)){
    this->setPinD(value.toInt());
  }
  value= parseProperty(json, "ty");
  if(isNotNull(value)){
    this->type= value.toInt();
  } 
  value= parseProperty(json, "st");
  if(isNotNull(value)){
    if(value == "1"){
      this->begin();
    }else{
      this->stop();
    }
  }
  //Acciones
  value= parseProperty(json, "sta");
  if(isNotNull(value)){
    this->set(value.toInt());
  }
}

//EEPROM
uint8_t DigitalControl::positions(){
	return 5;
}
void DigitalControl::readFromEeprom(int pos){
	uint8_t val= EEPROM.read(pos++);
	if(val == 1){
		this->pinD= EEPROM.read(pos++);
		this->type= EEPROM.read(pos++);
		this->begin();
	}
}
void DigitalControl::saveInEeprom(int pos){
	EEPROM.update(pos++, this->started);
	if(this->started){
		EEPROM.update(pos++, this->pinD);
		EEPROM.update(pos++, this->type);
	}
}
