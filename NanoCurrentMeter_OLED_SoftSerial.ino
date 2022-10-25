/* ATtiny85 NanoCurrentMeter 

#include <TinyOzOLED_decimalFix.h> 
#define voltSwitchPin 1
*/
#include <SoftwareSerial.h> // bu kütüphane datalogger gibi dış kaynaklı cihazlar ile haberleşme için kullanılabilir
SoftwareSerial tinySerial(-1, 4); //  tinySerial aygıtının D3 pinin (tx) yapıp RX pinin 1e çekiyoruz*/

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
  int Target = (Initial * 29) / 41; // tanımlanmamış olsa da hata mesajı vermektedir.
  
  do {
    Time = millis() - Start;
  } while (analogRead(A3) > Target && Time < 100000);

  OzOled.clearDisplay();
  
  if (digitalRead(voltSwitchPin)) {
 // nA = 1732868 / Time; // For 5Volts exactly
 nA = 1739799 / Time; // bu cihazda kullanılan 7805ten dolayı gelen 5.02V için tanımlanmıştır.;
  OzOled.printString("5V NanoAmmeter", 0,1);
  kOhms = 5020000/nA; // 5.02 V ile ohm yasasının uygulanması 
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

tinySerial.print(nA); // ham nA verisini birimsiz olarak serial Tx e yollar

OzOled.printString("kOhms= ", 0,5); OzOled.printNumber((long)kOhms);

/*
// buradaki iki satır debug için kullanılmaktadır.
OzOled.printString("nA= ", 0,5); OzOled.printNumber((long)nA);
OzOled.printString("Time= ", 0,6); OzOled.printNumber((long)Time);
*/

  pinMode(A3, OUTPUT);
  digitalWrite(A3, HIGH);
  for (;;);
}
