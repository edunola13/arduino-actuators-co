//#include "WProgram.h"
#include <Arduino.h>
//#include <ElementAbstract.h>
//#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.h"
#include "../arduino-basic-oo/ElementAbstract.h"
#include "LedAlert.h"

LedAlert::LedAlert(){}
LedAlert::LedAlert(uint8_t pinRed, uint8_t pinYellow, uint8_t pinGreen){
  this->begin(pinRed, pinYellow, pinGreen);
}
void LedAlert::begin(uint8_t pinRed, uint8_t pinYellow, uint8_t pinGreen) {  
  this->pinRed= pinRed;
  this->pinYellow= pinYellow;
  this->pinGreen= pinGreen;
  this->begin();
}
void LedAlert::begin() {
  if(pinRed != -1){
  pinMode(this->pinRed, OUTPUT);
  }
  if(pinYellow != -1){
    pinMode(this->pinYellow, OUTPUT);
  }
  if(pinGreen != -1){
    pinMode(this->pinGreen, OUTPUT);
  }  
  this->started= true;
}
void LedAlert::stop() {
  this->started= false;
}

uint8_t LedAlert::getCode(){
  return 103;
}

void LedAlert::set(uint8_t red, uint8_t yellow, uint8_t green){
  if(pinRed != -1){
  digitalWrite(pinRed, red);
  }
  if(pinYellow != -1){
    digitalWrite(pinYellow, yellow);
  }
  if(pinGreen != -1){
    digitalWrite(pinGreen, green);
  }  
}

void LedAlert::turnOnRed(){
  if(pinRed != -1){
  digitalWrite(pinRed, HIGH);
  }
}
void LedAlert::turnOffRed(){
  if(pinRed != -1){
  digitalWrite(pinRed, LOW);
  }
}
void LedAlert::turnOnYellow(){
  if(pinYellow != -1){
    digitalWrite(pinYellow, HIGH);
  }
}
void LedAlert::turnOffYellow(){
  if(pinYellow != -1){
    digitalWrite(pinYellow, LOW);
  }
}
void LedAlert::turnOnGreen(){
  if(pinGreen != -1){
    digitalWrite(pinGreen, HIGH);
  }
}
void LedAlert::turnOffGreen(){
  if(pinGreen != -1){
    digitalWrite(pinGreen, LOW);
  }
}

void LedAlert::jsonProperties(String &value){
  value+= this->propertyToJson("st", String(this->started), false, false);
  value+= this->propertyToJson("pinR", String(this->pinRed), false, false);
  value+= this->propertyToJson("pinY", String(this->pinYellow), false, false);
  value+= this->propertyToJson("pinG", String(this->pinGreen), false, true);
}
void LedAlert::parseJson(String* json){
  String value= parseProperty(json, "pinR");
  if(isNotNull(value)){
    this->pinRed= value.toInt();
  }
  value= parseProperty(json, "pinY");
  if(isNotNull(value)){
    this->pinYellow= value.toInt();
  }
  value= parseProperty(json, "pinG");
  if(isNotNull(value)){
    this->pinGreen= value.toInt();
  }
  //Start / Stop
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
  String pin= parseProperty(json, "pin");
  if(isNotNull(pin)){
    if(pin == "r"){
      if(value == "1"){
        this->turnOnRed();
      }else{
        this->turnOffRed();
      }
    }
    if(pin == "y"){
      if(value == "1"){
        this->turnOnYellow();
      }else{
        this->turnOffYellow();
      }
    }
    if(pin == "g"){
      if(value == "1"){
        this->turnOnGreen();
      }else{
        this->turnOffGreen();
      }
    }
  }
  }
}
//EEPROM
uint8_t LedAlert::positions(){
  return 6;
}
void LedAlert::readFromEeprom(int pos){
  uint8_t val= EEPROM.read(pos++);
  if(val == 1){
    this->pinRed= EEPROM.read(pos++);
    this->pinYellow= EEPROM.read(pos++);
    this->pinGreen= EEPROM.read(pos++);
    this->begin();
  }
}
void LedAlert::saveInEeprom(int pos){
  EEPROM.update(pos++, this->started);
  if(this->started){
    EEPROM.update(pos++, this->pinRed);
    EEPROM.update(pos++, this->pinYellow);
    EEPROM.update(pos++, this->pinGreen);
  }
}

//
//RGBA
LedRGBAlert::LedRGBAlert(){}
LedRGBAlert::LedRGBAlert(uint8_t pinRed, uint8_t pinGreen, uint8_t pinBlue){
  this->begin(pinRed, pinGreen, pinBlue);
}
void LedRGBAlert::begin(uint8_t pinRed, uint8_t pinGreen, uint8_t pinBlue) {
  this->pinRed= pinRed;
  this->pinGreen= pinGreen;
  this->pinBlue= pinBlue;
  this->begin();
}
void LedRGBAlert::begin() {
  pinMode(this->pinRed, OUTPUT);
  pinMode(this->pinGreen, OUTPUT);
  pinMode(this->pinBlue, OUTPUT);
  this->started= true;
}
void LedRGBAlert::stop() {
  this->started= false;
}

uint8_t LedRGBAlert::getCode(){
  return 104;
}

void LedRGBAlert::set(uint8_t red, uint8_t green, uint8_t blue){
  analogWrite(pinRed , red);
  analogWrite(pinGreen, green);
  analogWrite(pinBlue, blue); 
}

void LedRGBAlert::turnOnRed(){
  set(255, 0, 0);
}
void LedRGBAlert::turnOffRed(){
  set(0, 0, 0);
}
void LedRGBAlert::turnOnYellow(){
  set(255, 255, 0);
}
void LedRGBAlert::turnOffYellow(){
  set(0, 0, 0);
}
void LedRGBAlert::turnOnGreen(){
  set(0, 255, 0);
}
void LedRGBAlert::turnOffGreen(){
  set(0, 0, 0);
}

void LedRGBAlert::jsonProperties(String &value){
  value+= this->propertyToJson("st", String(this->started), false, false);
  value+= this->propertyToJson("pinR", String(this->pinRed), false, false);
  value+= this->propertyToJson("pinG", String(this->pinGreen), false, false);
  value+= this->propertyToJson("pinB", String(this->pinBlue), false, true);
}
void LedRGBAlert::parseJson(String* json){
  String value= parseProperty(json, "pinR");
  if(isNotNull(value)){
    this->pinRed= value.toInt();
  }  
  value= parseProperty(json, "pinG");
  if(isNotNull(value)){
    this->pinGreen= value.toInt();
  }
  value= parseProperty(json, "pinB");
  if(isNotNull(value)){
    this->pinBlue= value.toInt();
  }
  //Start / Stop
  value= parseProperty(json, "st");
  if(isNotNull(value)){
      if(value == "1"){
      this->begin();
    }else{
      this->stop();
    }
  }
  //Acciones
  //Prende apaga un color especifico
  value= parseProperty(json, "sta");
  if(isNotNull(value)){
  String pin= parseProperty(json, "pin");
  if(isNotNull(pin)){
    if(pin == "r"){
      if(value == "1"){
        this->turnOnRed();
      }else{
        this->turnOffRed();
      }
    }
    if(pin == "y"){
      if(value == "1"){
        this->turnOnYellow();
      }else{
        this->turnOffYellow();
      }
    }
    if(pin == "g"){
      if(value == "1"){
        this->turnOnGreen();
      }else{
        this->turnOffGreen();
      }
    }
  }
  }
  //Setea un color especifico
  String red= parseProperty(json, "red");
  String green= parseProperty(json, "green");
  String blue= parseProperty(json, "blue");
  if(isNotNull(red) && isNotNull(green) && isNotNull(blue)){
  this->set(red.toInt(), green.toInt(), blue.toInt());
  }
}
//EEPROM
uint8_t LedRGBAlert::positions(){
  return 6;
}
void LedRGBAlert::readFromEeprom(int pos){
  uint8_t val= EEPROM.read(pos++);
  if(val == 1){
    this->pinRed= EEPROM.read(pos++);
    this->pinGreen= EEPROM.read(pos++);
    this->pinBlue= EEPROM.read(pos++);
    this->begin();
  }
}
void LedRGBAlert::saveInEeprom(int pos){
  EEPROM.update(pos++, this->started);
  if(this->started){
    EEPROM.update(pos++, this->pinRed);   
    EEPROM.update(pos++, this->pinGreen);
    EEPROM.update(pos++, this->pinBlue);
  }
}
