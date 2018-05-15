# Multi-channel-Temperature-Data-Logger ❄ ☀ 

## Overview
Arduino based multi-channel K-type thermocouple data logger. Employs Arduino Uno and Adafruit's SD data logger shield to store temperature data in CSV. Configurable polling frequency and ability to echo to serial. Log files are sequentially stored according to boot. 

## Operation 
AD8495 outputs an analog voltage (75mV - 5V) with the relationship: 5 mV/°C. Arduino ADC used to interpret output voltage. ADC supply voltage corrected using Uno's internal 1.1V reference to normalize. 

Link to AD8495 datasheet: http://www.analog.com/media/en/technical-documentation/data-sheets/AD8494_8495_8496_8497.pdf

CSV indexes entries using SD shield's RTC, uses system time from OS when booted from USB (serial data port) 5V PSU.

Link to SD shield information: https://learn.adafruit.com/adafruit-data-logger-shield  

## Schematic
![](https://github.com/morganjlw/Multi-Channel-Temperature-Data-Logger/blob/master/Thermocouple-Array.png)


## PCB
![](https://github.com/morganjlw/Multi-Channel-Temperature-Data-Logger/blob/master/Thermocouple%20Amplifier%20Array%20PCB.JPG)


## BOM

 Item    | Qty.    | Components  | Link 
---------|---------|------------ |------------
Arduino Uno  | 1 | MCU |  https://www.adafruit.com/product/50
ADA SD Shield  | 1 | SD Shield | https://www.adafruit.com/product/1141
K-type Thermocouple | 4 | Probe | https://www.rpelectronics.com/tl-190-k-type-thermocouple-probe.html 
AD8495ARMZ-PKG:MSOP-8  | 4 | U1-U4 | https://ca.mouser.com/ProductDetail/Analog-Devices/AD8495ARMZ/?qs=sGAEpiMZZMv9Q1JI0Mo%2ftQIKSaidM5yv 
Header-Female-2.5 4_1x6  | 1 | P1 | N/A    
Header-Male-2.54_1x2 | 1 | P2,P3,P4,P5 | N/A
Thermocouple Jack (MPJ-K-F)  | 4 | TC1-TC4 | https://www.omega.ca/pptst_eng/MPJ.html 
Thermocouple Snap Strip (MPJ-K-F)  | 1 | TC1-TC4 | https://www.omega.ca/pptst_eng/MSS_Series.html
Capacitor - 100nF | 13 | C1-C13 | N/A
Capacitor - 10uF | 1 | C14 | N/A
Resistor - 100Ω | 1 | R1-R8 | N/A
 
 
 
