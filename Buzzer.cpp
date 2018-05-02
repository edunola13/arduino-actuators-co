//#include "WProgram.h"
#include <Arduino.h>
//#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
#include "../arduino-basic-oo/ElementAbstract.h"
#include "Buzzer.h"

//Comentar esta linea si no se quiere ni que se compile la funcion tone() de arduino
//ya que suele tener conflicto con otras librerias.
#define DEFINE_TONE

Buzzer::Buzzer(){}
Buzzer::Buzzer(uint8_t pin, uint8_t type) {
  this->begin(pin, type);
}
void Buzzer::begin(uint8_t pin, uint8_t type) {  
  this->pin= pin;
  this->type= type;
  this->begin();
}
void Buzzer::begin() {
  pinMode(this->pin, OUTPUT);
  this->started= true;  
}
void Buzzer::stop() {
  this->started= false;
}

uint8_t Buzzer::getCode(){
	return 102;
}

void Buzzer::soundTone(uint16_t ton){
  if(this->type == 1){
	#ifdef DOMO_DEBUG
    tone(pin, ton);
	#endif
  }
}
void Buzzer::soundTone(uint16_t ton, uint16_t duration){
  if(this->type == 1){
    #ifdef DOMO_DEBUG
    tone(pin, ton, duration);
	#endif
  }
  else{
	for (long i = 0; i < duration * 1000L; i += ton * 2) {
      digitalWrite(pin, HIGH);
      delayMicroseconds(ton);
      digitalWrite(pin, LOW);
      delayMicroseconds(ton);
    }
  }
}
void Buzzer::turnOut(){
  if(this->type == 1){
	#ifdef DOMO_DEBUG
    noTone(pin);
	#endif
  }
}

uint8_t Buzzer::getPin(){
  return pin;
}
void Buzzer::setPin(uint8_t pin){
  this->pin= pin;
  this->begin();
}

//JSON
void Buzzer::jsonProperties(String &value){
  value+= this->propertyToJson("st", String(this->started), false, false);
  value+= this->propertyToJson("ty", String(this->type), false, false);
  value+= this->propertyToJson("pin", String(this->pin), false, true);
}

void Buzzer::parseJson(String* json){
  //Atributos Simples
  String value= this->parseProperty(json, "pin");
  if(isNotNull(value)){
    this->setPin(value.toInt());
  } 
  //Start / Stop
  value= this->parseProperty(json, "st");
  if(isNotNull(value)){
    if(value == "1"){
		this->begin();
	}else{
		this->stop();
	}
  }
  value= this->parseProperty(json, "ty");
  if(isNotNull(value)){
    this->type= value.toInt();
  }
  //Acciones
  value= this->parseProperty(json, "sta");
  if(isNotNull(value)){
	if(value == "1"){
	  String tone= this->parseProperty(json, "to", 5);
	  String duration= this->parseProperty(json, "du", 5);
	  if(isNotNull(tone)){
		if(isNotNull(duration)){
		  this->soundTone(tone.toInt(), duration.toInt());
		}else{
		  this->soundTone(tone.toInt());
		}
	  }
	}else{
	  this->turnOut();
	}
  }
}
//EEPROM
uint8_t Buzzer::positions(){
	return 3;
}
void Buzzer::readFromEeprom(int pos){
	uint8_t val= EEPROM.read(pos++);
	if(val == 1){
		this->pin= EEPROM.read(pos++);
		this->type= EEPROM.read(pos++);
		this->begin();
	}
}
void Buzzer::saveInEeprom(int pos){
	EEPROM.update(pos++, this->started);
	if(this->started){
		EEPROM.update(pos++, this->pin);
		EEPROM.update(pos++, this->type);
	}
}

//
//ControlBuzzerInterval
ControlBuzzerInterval::ControlBuzzerInterval(){}
ControlBuzzerInterval::ControlBuzzerInterval(Buzzer *buzzer, SensorInterface *sensor, uint16_t tone){
  this->begin(buzzer, sensor, tone);
}
void ControlBuzzerInterval::begin(){
  if(this->buzzerCode != 0 && this->sensorCode != 0){
	this->started= true;
	this->active= false;
  } 
}
void ControlBuzzerInterval::begin(Buzzer *buzzer, SensorInterface *sensor, uint16_t tone){
  this->buzzer= buzzer;
  this->sensor= sensor;
  this->tone= tone;
  this->begin();
}
void ControlBuzzerInterval::stop(){
  this->started= false;
  this->active= false;
  this->buzzer->turnOut(); 
}

uint8_t ControlBuzzerInterval::getCode(){
	return 202;
}

bool ControlBuzzerInterval::updateStatus(){
  //Actualizo el sensor
  this->sensor->updateSensor();
  //Guardo estado anterior
  bool prevActive= this->active;
  //Veo si se supero el umbral
  if(this->sensor->getLevel(this->typeLevel) > this->max || this->sensor->getLevel(this->typeLevel) < this->min){
	this->active= true;
  }else{
	this->active= false;
  }
  //Analizo si hago sonar o no la alarma
  if(this->active){
	if(!prevActive || this->repeat){
		if(this->duration > 0){
			this->buzzer->turnOut();
			this->buzzer->soundTone(this->tone, this->duration);
		}else{
			this->buzzer->turnOut();
			this->buzzer->soundTone(this->tone);
		}
	}
  }
  if(!this->active && prevActive){
	this->buzzer->turnOut();
  }  
  //Retorno si hubo cambio de estado o no
  return prevActive != this->active;
}

uint8_t ControlBuzzerInterval::getBuzzerId(){
	return this->buzzerId;
}
uint8_t ControlBuzzerInterval::getBuzzerCode(){
	return this->buzzerCode;
}
uint8_t ControlBuzzerInterval::getSensorId(){
	return this->sensorId;
}
uint8_t ControlBuzzerInterval::getSensorCode(){
	return this->sensorCode;
}
void ControlBuzzerInterval::setBuzzer(Buzzer *buzzer, uint8_t id){
  this->buzzer= buzzer;  
  this->buzzerId= id;
  this->buzzerCode= this->buzzer->getCode();
}
void ControlBuzzerInterval::setSensor(SensorInterface *sensor, uint8_t id){
  this->sensor= sensor;
  this->sensorId= id;
  this->sensorCode= this->sensor->getCode();
}
bool ControlBuzzerInterval::getActive(){
  return this->active;
}

//JSON
void ControlBuzzerInterval::jsonProperties(String &value){
  if(this->buzzer != NULL && this->sensor != NULL){
	value+= this->propertyToJson("st", String(this->started), false, false);
	value+= this->propertyToJson("max", String(this->max), false, false);
	value+= this->propertyToJson("min", String(this->min), false, false);
	value+= this->propertyToJson("tyL", String(this->typeLevel), false, false);
	value+= this->propertyToJson("to", String(this->tone), false, false);
	value+= this->propertyToJson("du", String(this->duration), false, false);
	value+= this->propertyToJson("re", String(this->repeat), false, false);
	value+= this->propertyToJson("act", String(this->active), false, false);
	value+= this->propertyToJson("acId", String(this->buzzerId), false, false);
	value+= this->propertyToJson("acCo", String(this->buzzerCode), false, false);
	value+= this->propertyToJson("seId", String(this->sensorId), false, false);
	value+= this->propertyToJson("seCo", String(this->sensorCode), false, false);
	value+= this->propertyToJson("seLe", String(this->sensor->getLevel(this->typeLevel)), false, true);
  }else{
	value+= this->propertyToJson("st", String(this->started), false, false);
	value+= this->propertyToJson("max", String(this->max), false, false);
	value+= this->propertyToJson("min", String(this->min), false, false);
	value+= this->propertyToJson("tyL", String(this->typeLevel), false, false);
	value+= this->propertyToJson("to", String(this->tone), false, false);
	value+= this->propertyToJson("du", String(this->duration), false, false);
	value+= this->propertyToJson("re", String(this->repeat), false, false);
	value+= this->propertyToJson("act", String(this->active), false, true);
  }
}

void ControlBuzzerInterval::parseJson(String* json){
  String value= this->parseProperty(json, "re");   
  if(isNotNull(value)){
	bool val= value == "1";
    this->repeat= val;
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
  value= this->parseProperty(json, "to", 5);
  if(isNotNull(value)){
    this->tone= value.toInt();
  }
  value= this->parseProperty(json, "du", 5);
  if(isNotNull(value)){
    this->duration= value.toInt();
  }
  value= this->parseProperty(json, "act");
  if(isNotNull(value)){
	bool val= value == "1";
    this->active= val;
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
uint8_t ControlBuzzerInterval::positions(){
	return 18;
}
void ControlBuzzerInterval::readFromEeprom(int pos){
	uint8_t val= EEPROM.read(pos++);
	if(val == 1){	
		this->max= EEPROMReadInt(pos); pos+= 2;
		this->min= EEPROMReadInt(pos); pos+= 2;
		this->typeLevel= EEPROM.read(pos++);
		this->tone= EEPROMReadInt(pos); pos+= 2;
		this->duration= EEPROMReadInt(pos); pos+= 2;
		this->repeat= EEPROM.read(pos++);
		this->active= EEPROM.read(pos++);
		this->buzzerId= EEPROM.read(pos++);
		this->buzzerCode= EEPROM.read(pos++);
		this->sensorId= EEPROM.read(pos++);
		this->sensorCode= EEPROM.read(pos++);
		this->begin();
	}
}
void ControlBuzzerInterval::saveInEeprom(int pos){
	EEPROM.update(pos++, this->started);	
	if(this->started){
		EEPROMWriteInt(pos, this->max); pos+= 2;
		EEPROMWriteInt(pos, this->min); pos+= 2;
		EEPROM.update(pos++, this->typeLevel);	
		EEPROMWriteInt(pos, this->tone); pos+= 2;
		EEPROMWriteInt(pos, this->duration); pos+= 2;
		EEPROM.update(pos++, this->repeat);
		EEPROM.update(pos++, this->active);
		EEPROM.update(pos++, this->buzzerId);
		EEPROM.update(pos++, this->buzzerCode);
		EEPROM.update(pos++, this->sensorId);
		EEPROM.update(pos++, this->sensorCode);
	}
}
