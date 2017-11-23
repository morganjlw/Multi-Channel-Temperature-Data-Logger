### Multi-channel-Temperature-Data-Logger

# Overview
Arduino based multi-channel K-type thermocouple data logger. Employs Arduino Uno and Adafruit's SD data logger shield to store temperature data in CSV. Configurable polling frequency and ability to echo to serial. Log files are sequentially stored according to boot.

CSV indexes entries using SD shield's RTC, uses system time from OS when booted from USB (serial data port) 5V PSU. 

## Operation 
AD8495 outputs an analog voltage (75mV - 5V) with the realtionship: 5 mV/°C. Arduino ADC used to interpret output voltage. ADC supply voltage corrected using Uno's internal 1.1V reference to normalize. 

Link to AD8495 data sheet: http://www.analog.com/media/en/technical-documentation/data-sheets/AD8494_8495_8496_8497.pdf

# Schematic
![(Thermocouple Array Schematic)](https://github.com/morganjlw/Multi-Channel-Temperature-Data-Logger/blob/master/Thermocouple-Array.png)


# PCB
![(Thermocouple Array PCB)](https://github.com/morganjlw/Multi-Channel-Temperature-Data-Logger/blob/master/Thermocouple%20Amplifier%20Array%20PCB.JPG)


## BOM

 Item    | Qty.    | Components 
---------|---------|------------
Arduino Uno  | 1 | MCU  
ADA SD Shield  | 1 | SD Shield
K-type Thermocouple | 4 | Probe   
AD8495ARMZ-PKG:MSOP-8  | 4 | U1-U4   
Header-Female-2.5 4_1x6  | 1 | P1      
Header-Male-2.54_1x2 | 1 | P2,P3,P4,P5
Thermocouple Jack  | 4 | TC1-TC4   
Capacitor - 100nF | 13 | C1-C13   
Capacitor - 10uF | 1 | C14
Resistor - 100Ω | 1 | R1-R8
 
 
 
