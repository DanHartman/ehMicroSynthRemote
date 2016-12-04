#include <Adafruit_NeoPixel.h>

#define NUMPIXELS        3

#ifdef REMOTE_STOMP
  #define WSTX PIN_D8
#else
  #define WSTX PIN_D3
#endif
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, WSTX, NEO_GRB + NEO_KHZ800);

void ws_init(void) {
	pixels.begin();
}

void RGB_string(byte index, byte r, byte g, byte b) {
	pixels.setPixelColor(0, pixels.Color(r, g, b));
	pixels.setPixelColor(1, pixels.Color(r, g, b));
	pixels.setPixelColor(2, pixels.Color(r, g, b));
  pixels.show();
}