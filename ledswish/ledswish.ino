
#include <Adafruit_NeoPixel.h>

// Led strip properties
#define DATA_PIN        2   // Data pin for led data
#define NUM_PIXELS      31  // Number of leds on the strip

// Boost (white 'swish') properties
#define BOOST_WIDTH             NUM_PIXELS / 3  // Width of boost pattern in leds
#define BOOST_MAXVAL            120 // Max boost value
#define BOOST_STEP_INTERVAL     50  // Time (ms) between update steps of the swish
#define BOOST_REPEAT_INTERVAL   500 // Time (ms) between end of a swish and start of another

// Background gradient
#define BG_A_R          10
#define BG_A_G          186
#define BG_A_B          206

#define BG_B_R          0
#define BG_B_G          195
#define BG_B_B          97

int boostPosition = -BOOST_WIDTH;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show(); 
}

void loop() {

  // Color the leds
  int gradientMin = boostPosition - BOOST_WIDTH;
  int gradientMax = boostPosition + BOOST_WIDTH;
  for (int i = 0; i < NUM_PIXELS; i++) {  

    // Set base gradient color
    int r = getGradientValue(i, 0, NUM_PIXELS, BG_A_R, BG_B_R); 
    int g = getGradientValue(i, 0, NUM_PIXELS, BG_A_G, BG_B_G);  
    int b = getGradientValue(i, 0, NUM_PIXELS, BG_A_B, BG_B_B); 

    // Boost colors if the swish is here
    if (i > gradientMin && i < gradientMax) {
      int boostVal = (int) (sin(mapF(i, gradientMin, gradientMax, 0, PI)) * BOOST_MAXVAL);
      r = boostColor(r, boostVal);
      g = boostColor(g, boostVal);
      b = boostColor(b, boostVal);
    }

    // Apply color
    strip.setPixelColor(i, r, g, b);   
    
  }
  strip.show();

  // Handle updating of the boost 'swish'
  if (boostPosition < NUM_PIXELS + BOOST_WIDTH) {
    boostPosition++;
    delay(BOOST_STEP_INTERVAL);
  } else {
    boostPosition = -BOOST_WIDTH;
    delay(BOOST_REPEAT_INTERVAL);
  }

}

/**
 * Boost the [color] with [boost] (clamped to [color, 255]
 */
int boostColor(int color, int boost) {
  return min(color + boost, 255);
}

/**
 * Get gradient value of gradient colorA - colorB where
 * the point of the returned color is val in [lowerBound, upperBound]
 */
int getGradientValue(int val, int lowerBound, int upperBound, int colorA, int colorB) {
   return (int) (
     (float) colorA 
     + (float) (val - lowerBound) / (float) (upperBound - lowerBound) 
     * (float) (colorB - colorA)
   );
}

/**
 * Map x in in_range to out range in floating point precision
 */
float mapF(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



