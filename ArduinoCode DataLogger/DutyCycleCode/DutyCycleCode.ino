/*  rcTiming.ino -- JW, 30 November 2015 -- 
 * Uses pin-change interrupts on A0-A4 to time RC pulses
 *
 * Ref: http://arduino.stackexchange.com/questions/18183/read-rc-receiver-channels-using-interrupt-instead-of-pulsein
 *
 */
#include <Streaming.h>
#include "Adafruit_MAX31855.h"
#include <SPI.h>

// digital IO pins.
#define MAXDO   3
#define MAXCStIN   8
#define MAXCStOUT   6
#define MAXCLK  5

// initialize the Thermocouple
Adafruit_MAX31855 tempIn(MAXCLK,  MAXCStIN, MAXDO);
Adafruit_MAX31855 tempOut(MAXCLK, MAXCStOUT, MAXDO);

// define variables for thermocouple
double Tin, Tout;

//define variables for pulse counter for massflow meter
static   byte rcOld;        // Prev. states of inputs
volatile unsigned long rcRises[4]; // times of prev. rising edges
volatile unsigned long rcTimes[4]; // recent pulse lengths
volatile unsigned long rcPulse[4]; // recent pulse lengths
volatile unsigned int  rcChange=0; // Change-counter
bool SCLl = false;


// Be sure to call setup_rcTiming() from setup()
void setup_rcTiming() {
  rcOld = 0;
  pinMode(A0, INPUT);  // pin 14, A0, PC0, for pin-change interrupt
  pinMode(A1, INPUT);  // pin 15, A1, PC1, for pin-change interrupt
  PCMSK1 |= 0x01;       // One-bit mask for one channels
  PCIFR  |= 0x02;       // clear pin-change interrupts if any
  PCICR  |= 0x02;       // enable pin-change interrupts
}

// Define the service routine for PCI vector 1
// Uses Pin A0 (and A1 but who cares about A1) to detect pulses on a 
// square wave signal

ISR(PCINT1_vect) {
  byte rcNew = PINC & 15;   // Get low 1 bits, A0
  byte changes = rcNew^rcOld;   // Notice changed bits
  byte channel = 0;
  unsigned long now = micros(); // micros() is ok in int routine
  while (changes) {
    if ((changes & 1)) {  // Did current channel change?
      if ((rcNew & (1<<channel))) { // Check rising edge
        rcTimes[channel] = now-rcRises[channel];    
      } else {              // Is falling edge
        rcPulse[channel] = now - rcRises[channel];
        rcRises[channel] = now; // Is Falling edge
      }
    }
    changes >>= 1;      // shift out the done bit
    ++channel;
    ++rcChange;
  }
  rcOld = rcNew;        // Save new state
}

void setup() {
  // Set load pin to output
  
  Serial.begin(115200);
  Serial.println("Starting RC Timing Test");
  setup_rcTiming();

}


void loop() {
  
  unsigned long rcT; // copy of recent pulse lengths
  unsigned long rcR; // copy of recent pulse lengths
  unsigned long rcP; // copy of recent pulse lengths
  unsigned int rcN;
   
  if (rcChange) {

    // Data is subject to races if interrupted, so off interrupts
    cli();          // Disable interrupts
    rcN = rcChange;
    rcChange = 0;       // Zero the change counter
    rcT = rcTimes[0];
    rcR = rcRises[0];
    rcP = rcPulse[0];
    }
   Tin = tempIn.readCelsius();
   Tout = tempOut.readCelsius();
   Serial << " " << Tin <<" \t"<< Tout  <<" \t"<< rcT << endl;
   sei();          // reenable interrupts
   
   delay(100); 
    }
    
