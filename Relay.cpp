//#include "WProgram.h"
#include <Arduino.h>
//#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
#include "../arduino-basic-oo/ElementAbstract.h"
#include "Relay.h"

Relay::Relay(){
}
Relay::Relay(uint8_t pin){
  this->begin(pin);
}
void Relay::begin(uint8_t pin){
  this->pin= pin;
  this->begin();
}
void Relay::begin(){
  this->started= true;
  pinMode(this->pin, OUTPUT);
}
void Relay::stop(){
  this->started= false;
  this->commonlyClose();
}

uint8_t Relay::getCode(){
	return 101;
}

void Relay::commonlyOpen(){
  digitalWrite(pin, HIGH); 
  this->state= HIGH;
}
void Relay::commonlyClose(){      
  digitalWrite(pin, LOW); 
  this->state= LOW;
}
void Relay::changeStatus(){
  if(this->state == HIGH){
    commonlyClose();	
  }else{
    commonlyOpen(); 
  }
}

uint8_t Relay::getPin(){
  return pin;
}
void Relay::setPin(uint8_t pin){
  commonlyClose();
  this->pin= pin;
  pinMode(this->pin, OUTPUT);
  commonlyClose();
}
uint8_t Relay::getState(){
  return state;
}

//JSON
void Relay::jsonProperties(String &value){
  value+= this->propertyToJson("st", String(this->started), false, false);
  value+= this->propertyToJson("pin", String(this->pin), false, false);
  value+= this->propertyToJson("sta", String(this->state), false, true);
}

void Relay::parseJson(String* json){
  String value= this->parseProperty(json, "pin");
  if(isNotNull(value)){
    this->setPin(value.toInt());
  }
  value= this->parseProperty(json, "st");
  if(isNotNull(value)){
    if(value == "1"){
		this->begin();
	}else{
		this->stop();
	}
  }
  //Acciones
  value= this->parseProperty(json, "sta");
  if(isNotNull(value)){
    if(value == "1"){
	  commonlyOpen();
    }else{
      commonlyClose();
	}
  }  
}
//EEPROM
uint8_t Relay::positions(){
	return 3;
}
void Relay::readFromEeprom(int pos){
	uint8_t val= EEPROM.read(pos++);
	if(val == 1){
		this->pin= EEPROM.read(pos++);
		this->begin();
	}
}
void Relay::saveInEeprom(int pos){
	EEPROM.update(pos++, this->started);
	if(this->started){
		EEPROM.update(pos++, this->pin);
	}
}

//
//ControlRelayInterval
ControlRelayInterval::ControlRelayInterval(){}
ControlRelayInterval::ControlRelayInterval(Relay *relay, SensorInterface *sensor, bool invert){
  this->begin(relay, sensor, invert);
}
void ControlRelayInterval::begin(){
  if(this->relayCode != 0 && this->sensorCode != 0){
	this->started= true;
	this->active= false;
  }  
}
void ControlRelayInterval::begin(Relay *relay, SensorInterface *sensor, bool invert){
  this->relay= relay;
  this->sensor= sensor;
  this->invert= invert;
  this->begin();
}
void ControlRelayInterval::stop(){
  this->started= false;
  this->active= false;
  this->relay->commonlyClose(); 
}

uint8_t ControlRelayInterval::getCode(){
	return 201;
}

bool ControlRelayInterval::updateStatus(){
  //Actualizo el sensor
  this->sensor->updateSensor();
  //Guardo estado anterior
  int prevActive= this->active;
  //Analizo si se supero el umbral
  if(this->sensor->getLevel(this->typeLevel) > this->max || this->sensor->getLevel(this->typeLevel) < this->min){
	if(this->sensor->getLevel(this->typeLevel) > this->max){
		this->active= 1;
	}else{
		this->active= -1;
	}
  }else{
	this->active= 0;
  }
  //Analizo si cambio o no el estado del relay
  if(this->active != 0 && this->active != prevActive){
	if(millis() - lastTime >= time || millis() - lastTime < 0){
	  if(this->active == 1){
		if(!this->invert){
		  this->relay->commonlyOpen();    
		}else{
		  this->relay->commonlyClose();   
		}
		//Actualizo el tiempo    
		this->lastTime= millis();
	  }
	  if(this->active == -1){
		if(!this->invert){
		  this->relay->commonlyClose();              
		}else{
		  this->relay->commonlyOpen();
		}
		//Actualizo el tiempo    
		this->lastTime= millis();
	  }	  
	}else{  
	  this->active= prevActive;
	}
  }
  //Retorno si hubo cambio de estado o no
  return this->active == 0 || prevActive != this->active;
}

uint8_t ControlRelayInterval::getRelayId(){
	return this->relayId;
}
uint8_t ControlRelayInterval::getRelayCode(){
	return this->relayCode;
}
uint8_t ControlRelayInterval::getSensorId(){
	return this->sensorId;
}
uint8_t ControlRelayInterval::getSensorCode(){
	return this->sensorCode;
}
void ControlRelayInterval::setRelay(Relay *relay, uint8_t id){
  this->relay= relay;  
  this->relayId= id;
  this->relayCode= this->relay->getCode();
}
void ControlRelayInterval::setSensor(SensorInterface *sensor, uint8_t id){
  this->sensor= sensor;
  this->sensorId= id;
  this->sensorCode= this->sensor->getCode();
}

//JSON
void ControlRelayInterval::jsonProperties(String &value){
  if(this->relay != NULL && this->sensor != NULL){
	value+= this->propertyToJson("st", String(this->started), false, false);
	value+= this->propertyToJson("inv", String(this->invert), false, false);
	value+= this->propertyToJson("max", String(this->max), false, false);
	value+= this->propertyToJson("min", String(this->min), false, false);
	value+= this->propertyToJson("tyL", String(this->typeLevel), false, false);
	value+= this->propertyToJson("ti", String(this->time), false, false);
	value+= this->propertyToJson("act", String(this->active), false, false);
	value+= this->propertyToJson("acId", String(this->relayId), false, false);
	value+= this->propertyToJson("acCo", String(this->relayCode), false, false);
	value+= this->propertyToJson("acSta", String(this->relay->getState()), false, false);
	value+= this->propertyToJson("seId", String(this->sensorId), false, false);
	value+= this->propertyToJson("seCo", String(this->sensorCode), false, false);
	value+= this->propertyToJson("seLe", String(this->sensor->getLevel(this->typeLevel)), false, true);
  }else{
	value+= this->propertyToJson("st", String(this->started), false, false);
	value+= this->propertyToJson("max", String(this->max), false, false);
	value+= this->propertyToJson("min", String(this->min), false, false);
	value+= this->propertyToJson("tyL", String(this->typeLevel), false, false);
	value+= this->propertyToJson("ti", String(this->time), false, false);
	value+= this->propertyToJson("act", String(this->active), false, false);
	value+= this->propertyToJson("inv", String(this->invert), false, true);
  }
  
}

void ControlRelayInterval::parseJson(String* json){
  String value= this->parseProperty(json, "inv");
  if(isNotNull(value)){
	bool val= value == "1";
    this->invert= val;
  }
  value= this->parseProperty(json, "max", 5);
  if(isNotNull(value)){
    this->max= value.toInt();
  }
  value= this->parseProperty(json, "min", 5);
  if(isNotNull(value)){
    this->min= value.toInt();
  }
  value= this->parseProperty(json, "tyL");
  if(isNotNull(value)){
    this->typeLevel= value.toInt();
  }
  value= this->parseProperty(json, "ti", 5);
  if(isNotNull(value)){
    this->time= value.toInt();
  }
  value= this->parseProperty(json, "act");
  if(isNotNull(value)){
    this->active= value.toInt();
  }
  value= this->parseProperty(json, "st");
  if(isNotNull(value)){
    if(value == "1"){
		this->begin();
	}else{
		this->stop();
	}
  }
   
  this->updateStatus();
}
//EEPROM
uint8_t ControlRelayInterval::positions(){
	return 15;
}
void ControlRelayInterval::readFromEeprom(int pos){
	uint8_t val= EEPROM.read(pos++);
	if(val == 1){	
		this->invert= EEPROM.read(pos++);
		this->max= EEPROMReadInt(pos); pos+= 2;
		this->min= EEPROMReadInt(pos); pos+= 2;
		this->typeLevel= EEPROM.read(pos++);
		this->time= EEPROMReadInt(pos); pos+= 2;
		this->relayId= EEPROM.read(pos++);
		this->relayCode= EEPROM.read(pos++);
		this->sensorId= EEPROM.read(pos++);
		this->sensorCode= EEPROM.read(pos++);
		this->begin();
	}
}
void ControlRelayInterval::saveInEeprom(int pos){
	EEPROM.update(pos++, this->started);	
	if(this->started){
		EEPROM.update(pos++, this->invert);	
		EEPROMWriteInt(pos, this->max); pos+= 2;
		EEPROMWriteInt(pos, this->min); pos+= 2;
		EEPROM.update(pos++, this->typeLevel);
		EEPROMWriteInt(pos, this->time); pos+= 2;
		EEPROM.update(pos++, this->relayId);
		EEPROM.update(pos++, this->relayCode);
		EEPROM.update(pos++, this->sensorId);
		EEPROM.update(pos++, this->sensorCode);
	}
}
