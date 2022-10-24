/* ATtiny85 NanoCurrent Meter with OLED

Inspired by
   David Johnson-Davies - www.technoblogy.com - 9th October 2019
    http://www.technoblogy.com/show?2S67#cite_note2
   Also: OneCircuit did a version of this: 
    https://onecircuit.blogspot.com/2021/05/0000-0000-0101-1111.html
 */

#include <TinyOzOLED_decimalFix.h> 
#define voltSwitchPin 1

#include <SoftwareSerial.h> // include library for transmitting the data serially to an external device like a datalogger
SoftwareSerial tinySerial(-1, 4); // declares tinySerial using D3 for Transmit (tx) but not Receive (pin -1) */

// Setup ********************************************** 
void setup() {
  pinMode(voltSwitchPin,INPUT_PULLUP);
  pinMode(4,OUTPUT);
  OzOled.init(); 

  tinySerial.begin(9600); 
}

void loop() {
 
  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  delay(500);
  pinMode(A3, INPUT);
  unsigned long Start = millis(), Time, nA, kOhms;
  unsigned int Initial = analogRead(A3);
  int Target = (Initial * 29) / 41; // JD_K: this used to be unsigned, but it gives an error message
  
  do {
    Time = millis() - Start;
  } while (analogRead(A3) > Target && Time < 100000);

  OzOled.clearDisplay();
  
  if (digitalRead(voltSwitchPin)) {
 // nA = 1732868 / Time; // For 5Volts exactly
 nA = 1739799 / Time; // For 5.02V which is the measured amount of the voltage regulator on this specific device;
  OzOled.printString("5V NanoAmmeter", 0,1);
  kOhms = 5020000/nA; // using the exact 5.02V of this device, and Ohms law to calculate resistance 
  } else {
  OzOled.printString("3V3 NanoAmmeter", 0,1); 
  nA = 1143693 / Time;
  kOhms = 3300000/nA; 
  }
  
  if (Time >= 100000) {    OzOled.printString("LO (under 30nA)", 0, 3); }
  else if (nA < 1000) {   OzOled.printNumber((long)nA, 0,3);  OzOled.printString(" nA"); }
  else if (nA < 10000) {  float uA=nA/1000.00; OzOled.printNumber((float)uA,2, 0,3);   OzOled.printString(" uA"); }
  else if (nA < 100000) {  float uA=nA/1000.00; OzOled.printNumber((float)uA,2, 0,3);   OzOled.printString(" uA"); }
  else {   OzOled.printString("HI (over 10uA)", 0,3); }

tinySerial.print(nA); // sends the raw nA value without units to serial Tx (pin D4)

OzOled.printString("kOhms= ", 0,5); OzOled.printNumber((long)kOhms);

/*
// these two lines were for debugging. Very useful
OzOled.printString("nA= ", 0,5); OzOled.printNumber((long)nA);
OzOled.printString("Time= ", 0,6); OzOled.printNumber((long)Time);
*/

  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  for (;;);
}
