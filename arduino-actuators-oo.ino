#include "Buzzer.h"
#include "DigitalControl.h"
#include "Relay.h"
#include "LedAlert.h"
//ESTO ES NECESARIO PORQUE CUANDO SE LLAMA LA LIBRERIA DESDE EL VENDOR NO AUTOCARGA EL CPP
#include "vendor/igniteit/arduino-basic-oo/JsonHelper.cpp"
#include "vendor/igniteit/arduino-basic-oo/ElementAbstract.cpp"

LedAlert s(2,3,4);
Buzzer buz(2);
DigitalControl dig(3,1);
Relay dis(3);

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

