/*
project name: smart moomin
current bugs: update rate, brightness scaling, sensor scaling
, only accurate w color light scoure
add feature: add button, create all in one board
*/
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>
#include <stdio.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 60 // Popular NeoPixel ring size

#define DELAYVAL 0 // Time (in milliseconds) to pause between pixels

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);

 // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
 // END of Trinket-specific code.
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

  // Now we're ready to get readings!
}


void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux; //Init variables at 0

  tcs.getRawData(&r, &g, &b, &c);

  uint8_t r8 = (uint8_t)(((uint32_t)r*255) / 65535);
  uint8_t g8 = (uint8_t)(((uint32_t)g*255) / 65535);
   uint8_t b8 = (uint8_t)(((uint32_t)b*255) / 65535);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
 
  //Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
 // Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R8: "); Serial.print(r8); Serial.print(" ");
  Serial.print("G8: "); Serial.print(g8); Serial.print(" ");
  Serial.print("B8: "); Serial.print(b8); Serial.print(" ");

  /*
  Serial.print("R: "); Serial.print(r); Serial.print(" ");
  Serial.print("G: "); Serial.print(g); Serial.print(" ");
  Serial.print("B: "); Serial.print(b); Serial.print(" ");
  */
  //Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");


    pixels.clear(); // Set all pixel colors to 'off'


  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:

   // pixels.setPixelColor(i, pixels.Color(r8, g8, b8 ));
    pixels.setPixelColor(i, pixels.Color(r8, g8,b8 ));
    
   //pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
  }
  pixels.show(); // Send the updated pixel colors to the hardware. outside the loop so it doesn't blink.

}
