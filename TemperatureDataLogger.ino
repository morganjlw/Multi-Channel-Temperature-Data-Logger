/*
   == Multi-channel Temperature Data Logger ==
   Written by: Morgan Williams (23/11/2017)

   Summary: Rudimentary program to normalize 2 LM35 sensor outputs and write to an
   SD .CSV file for optimal portablility and manipulation.

   Program Notes:
   - Temperature acquisition method is external to main polling mechanism to enhance scalability.
   - AD8495 analog voltage output is linearly proportional to temperature: 1 degree C ~ 5 mV
   - ~3s sensor polling.
   - When SD card is full, indicator LED will flash accoringly.
*/

//Libraries
#include <SD.h>
#include <Wire.h>
#include <SPI.h>
#include <RTClib.h>
RTC_PCF8523 rtc; // define rtc object

//Macros
#define redLED1 4
#define greenLED2 5
#define TSENSOR1 0
#define TSENSOR2 1
#define TSENSOR3 2
#define TSENSOR4 3

#define LOG_INTERVAL 3000 // Set polling frequency (3000 = 3s)
#define SYNC_INTERVAL 3000 // Set polling frequency (3000 = 3s)
#define ECHO_TO_SERIAL 1// echo data to serial port, for display on the serial monitor

const int chipSelect = 10;
uint32_t syncTime = 0; // Time of last sync()

File logfile; // Logging file

//Globals
float C1 = 0.0; // channel temp
float C2 = 0.0; // channel temp
float C3 = 0.0; // channel temp
float C4 = 0.0; // channel temp

void setup() {

  pinInit(); // Pin assignment intialization

  Serial.begin(9600); // Start serial

  //Init blink sequence
  LEDBlink(redLED1, greenLED2, 0, 50, 150, 4); 
  delay(50);
  LEDBlink(redLED1, greenLED2, 1, 50, 150, 5);

  SDInit(); //Init SD 

  // Create a new file, increment if file exists
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i / 10 + '0';
    filename[7] = i % 10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE);
      break;
    }
  }

  Serial.print("Logging to: ");
  Serial.println(filename);

  Wire.begin();
  if (!rtc.begin()) {
    logfile.println("RTC failed");
  #if ECHO_TO_SERIAL
    Serial.println("RTC Failed");
  #endif 
  }
  
  logfile.print("Seconds, RTC, Channel 1, Channel 2, Channel 3, Channel 4");

  #if ECHO_TO_SERIAL
  Serial.println("Seconds, RTC, Channel 1, Channel 2, Channel 3, Channel 4");
  #endif 
  
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//adjust to system time

}

void loop() {

  DateTime now = rtc.now();

  delay((LOG_INTERVAL - 1) - (millis() % LOG_INTERVAL)); //log delay

  digitalWrite(greenLED2, HIGH); //signify writing to SD
  
  //get measurements from channels 1-4
  getTemp(TSENSOR1, C1);
  getTemp(TSENSOR2, C2);
  getTemp(TSENSOR3, C3);
  getTemp(TSENSOR4, C4);

  //get sensor vcc
  float vcc = readVcc();
  /*
  analogRead(TSENSOR1);
  delay(10);
  float ADCData = analogRead(TSENSOR1);
  float VCC = readVcc();
  //float VCC = 4.91;
  float voltage = ADCData * VCC / 1023 / 1000;
  float temp = (voltage * 100) + 0.5;
  */

   
  
  //write to SD in CSV format
  writeDataCSV (millis()/1000, DEC);
  writeDataCSV (C1, DEC);
  writeDataCSV (C1, DEC);
  writeDataCSV (C1, DEC);
  writeDataCSV (C1, DEC);
  writeDataCSV (vcc, DEC);
  logfile.println();

#if ECHO_TO_SERIAL
  //date
  Serial.print(millis()/1000, DEC);
  Serial.print(", ");
  Serial.print(now.year());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print("/");
  Serial.print(now.hour());
  Serial.print("/");
  Serial.print(now.minute());
  Serial.print("/");
  Serial.print(now.second());
  Serial.print(", ");

  //data
  Serial.print(vcc, DEC);
  Serial.print(", ");
  Serial.print(C1, DEC);
  Serial.print(", ");
  Serial.print(C2, DEC);
  Serial.print(", ");
  Serial.print(C3, DEC);
  Serial.print(", ");
  Serial.print(C4, DEC);
  Serial.println();
#endif 
  
  digitalWrite(greenLED2, LOW);

  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();

  // blink LED to show we are syncing data to the card & updating FAT!
  digitalWrite(redLED1, HIGH);
  logfile.flush();
  digitalWrite(redLED1, LOW);

}

void getTemp(int sensor, float channel){
  analogRead(sensor);
  delay(10);
  float ADCData = analogRead(sensor);
  float vcc = readVcc(); // Read normalized ADC supply voltage
  //float VCC = 4.91;
  float voltage = ADCData * vcc / 1023 / 1000; // Converting normalized ADC reading to mV
  channel = voltage * 50; // Linear relationship: (5mV/C)
}

// LED Blinking method
void LEDBlink(int LED_1, int LED_2, int BlinkFormat, int frequency, int wait, int cycles) {

  for (int cycles = 0; cycles < 4; cycles++) {
    switch (BlinkFormat) {
      case 0 : //synchronous blink
        digitalWrite(LED_1, HIGH );
        digitalWrite(LED_2, HIGH );
        delay(frequency);
        digitalWrite(LED_1, LOW );
        digitalWrite(LED_2, LOW );
        delay(wait);
        break;

      case 1: //rapid blink
        digitalWrite(LED_1, HIGH );
        digitalWrite(LED_2, LOW );
        delay(frequency);
        digitalWrite(LED_1, LOW );
        digitalWrite(LED_2, HIGH );
        delay(frequency);
        digitalWrite(LED_2, LOW );
        break;

      case 2: //LED 1 Rapid
        digitalWrite(LED_1, HIGH);
        delay(frequency);
        digitalWrite(LED_1, LOW);
    }
  }
}

// Reading VCC
long readVcc() {

  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both

  long result = (high << 8) | low;

  result = 1134041 / result; //compensating for internal 1V1; 1134041 = 1.1 * 1023 * 1000 * (4930/4892) -> (voltmeter calibration)

  //result = 1150145 / result; //compensating for internal 1V1; 1134041 = 1.1 * 1023 * 1000 * (5000/4892) -> (voltmeter calibration)

  return result; // Vcc in millivolts
}

// Pin configurations
void pinInit() {

  analogReference(DEFAULT); // Uses VCC as AREF

  pinMode(chipSelect, OUTPUT);

  //Setup the TSENSORs as analog inputs
  pinMode(TSENSOR1, INPUT); // T1
  pinMode(TSENSOR2,INPUT); // T2
  pinMode(TSENSOR3,INPUT); // T3
  pinMode(TSENSOR4,INPUT); // T4
}

//SD initializations
void SDInit () {

  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  LEDBlink(redLED1, greenLED2, 0, 150, 100, 2);
}

//method to print data in CSV format
// prints equivalent to ===> "data,_" (_ indicates space)
void writeDataCSV (float data, char type) {
  logfile.print(data, type);
  logfile.print(", ");
}


