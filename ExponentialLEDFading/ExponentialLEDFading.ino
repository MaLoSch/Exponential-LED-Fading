/*
 * Exponential LED Fading
*/

/* LIBRARIES */
#include "FastLED.h" // Include FastLED library http://fastled.io/

/* CONSTANT VARIABLES */
const int rgbLEDMatrixPin = 9; // Pin # where LED matrices are attached to
const int numPixelPerMatrix = 64; // # of pixels per LED matrix
const int numMatrices = 2; // # of LED matrices
const int totalPixels = numPixelPerMatrix * numMatrices; // calculate total number of pixels

/* LIGHT */
int c_hue = 0; // variable to store hue value of color
int c_sat = 255; // variable to store saturation value of color
int c_val = 0; // variable to store brightness value of color

/* EXPONENTIAL FADING */
const int maxFadingIntervals = 3000; // 3000 # of intervals needed for fading to reach max. brightness
float fadingFactor; // variable to store fading factor
int fadingPos = 0; // variable to store current position in fading cycle
int fadingShift = 2; // shift fading to avoid dark spots at lower end, and non-optimal brightness at upper end

/* NEOPIXEL */
CRGB rgbLEDMatrix[totalPixels]; // LED array of the size totalPixels

/* NON-BLOCKING */
unsigned long prevMillis = 0; // variable to store last time code was updated
unsigned long currentMillis = 0; // variable to store how long the code has been running for
const long intervalThreshold = 100; // variable to store threshold in which code will be updated

void setup() {
  Serial.begin(9600); // start serial port @9600 baud rate
  FastLED.addLeds<WS2812B, rgbLEDMatrixPin, GRB>(rgbLEDMatrix, totalPixels); // assign rgbLEDMatrix to FastLED object
  fadingFactor = ((maxFadingIntervals+fadingShift) * log10(2))/(log10(255)); // calculate fadingFactor as seen on https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms/
}

void loop() {
  currentMillis = millis(); // store time since sketch has been running (in milli seconds)
  if(currentMillis - prevMillis >= intervalThreshold) { // condition if code gets updated
    prevMillis = currentMillis; // store time from currentMillis into prevMillis

    /* CODE UPDATES */
    fade(); // calculate current fading value
    controlLEDS(); // control RGB LED matrices
  }
}

void fade() {
  c_val = pow(2,((fadingPos+fadingShift) / fadingFactor))-1; // calculate PWM value for LEDs based on current position in fading cycle
  Serial.println(c_val);
  fadingPos++; // increase fadingInterval by 1

  if(fadingPos >= maxFadingIntervals) {
    fadingPos = 0; // set fadingInterval back to 0 -> start over
  }
}

void controlLEDS() {
  for(int i=0; i<totalPixels; i++) { // cycle through all pixels
    rgbLEDMatrix[i] = CHSV(c_hue, c_sat, c_val); // set color of current pixel
  }
  FastLED.show(); // update RGB LED matrices
}
