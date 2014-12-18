#include <Adafruit_NeoPixel.h>

#define PIN 6

#define NUMPIXELS 47

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//define the ring mappings here (in the order we want them to light):
byte _outer[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
byte _middle[] = {24,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25};
byte _inner[] = {41,42,43,44,45,46,40};


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(12);
}

void loop() {

   outerLoop();
   middleLoop();
   innerLoop();

    barSpiral(false);
    barSpiral(true);
    barSpiral(false);
    barSpiral(true);
    fireWork(strip.Color(255, 120,2));
    fireWork(strip.Color(0, 250,0));
    fireWork(strip.Color(170, 0,255));
    fireWork(strip.Color(255,0,0));
    fireWork(strip.Color(255,255,255));
}

// All three rings circle and highlight a random pixel on each loop
void radar(){

  
  
}

void fireWork(uint32_t color){
// run up the middle,
// to the centre and flash rings outwards, 
// WITH A FADEOUT!

uint32_t faded = color / 2;
uint32_t orange = strip.Color(180, 120,1);
int vel = 150;

 strip.setPixelColor(12, orange);
 delay(vel);
  strip.show();  
 strip.setPixelColor(32, orange);
  delay(vel);
   strip.show();  
 strip.setPixelColor(44, orange);
  delay(vel);
   strip.show();  
   
 strip.setPixelColor(40, color);
 strip.setBrightness(36); 
delay(vel);
  strip.show();  
  
 //center 7 is the explosion:
 setInner(color);
 strip.setPixelColor(40, color);
 strip.setPixelColor(32, 0);
   strip.setPixelColor(12, 0);
 
  strip.show();  
  delay(vel);
  strip.setBrightness(12);
 strip.setPixelColor(40, 0);
    //shockwave the middle ring
 setMiddle(color);
 setInner(faded);
 strip.show(); 
 delay(vel);
 
 //shock the outer ring and fade the middle one, turn the centre one off:
  setOuter(color); 
  setMiddle(faded);
 setInner(0);
  strip.show(); 
 delay(vel);
 
  setOuter(faded); 
    setMiddle(0); 
    strip.show(); 
 delay(vel);
 
 setOuter(0); 
    strip.show(); 
 delay(vel);
};

void setInner(uint32_t colour){
  for (int i = 41; i < 47; i++) {
    strip.setPixelColor(i, colour);
 }
}

void setMiddle(uint32_t colour){
   for (int i = 24; i < 40; i++) {
  strip.setPixelColor(i, colour);
   } 
}

void setOuter(uint32_t colour){
    for (int i = 0; i < 24; i++) {
      strip.setPixelColor(i, colour);
   } 
}

void barSpiral(boolean leaveOn){

  byte steps[13][7]  = {
     {0, 24, 41, 40, 44, 32, 12}
    ,{1, 24, 41, 40, 44, 31, 13}
    ,{2, 39, 41, 40, 44, 31, 14}
    ,{3, 38, 42, 40, 45, 30, 15}
    ,{4, 38, 42, 40, 45, 30, 16}
    ,{5, 37, 42, 40, 45, 29, 17}
    ,{6, 36, 43, 40, 45, 29, 18}
    ,{7, 35, 43, 40, 46, 28, 19}
    ,{8, 34, 43, 40, 46, 27, 20}
    ,{9, 34, 43, 40, 46, 26, 21}
    ,{10, 33, 44, 40, 41, 26, 22}
    ,{11, 33, 44, 40, 41,25, 23}
  };

  byte c = 0;

  for (byte i = 0; i < 13; i++) {
    //new line, clear the old one and pause:
    if(!leaveOn){
    strip.clear();
    }
    
    for (byte j = 0; j < 7; j++) {
      strip.setPixelColor(steps[i][j], Wheel(c));  
      c++;     
    }
    strip.show();    
    delay(50);
  }
      strip.clear();
}

void outerLoop(){

  for(byte i=0; i < 24; i++) {
    strip.setPixelColor(_outer[i], strip.Color(255,0,0));
    strip.show();
    delay(100);
  }
}

void middleLoop(){

  for(byte j=0; j < 16; j++) {
    strip.setPixelColor(_middle[j], strip.Color(0,255,0));
    strip.show();
    delay(100);
  }
}

void innerLoop(){
 
  for(byte k=0; k < 7; k++) {
    strip.setPixelColor(_inner[k], strip.Color(0,0,255));
    strip.show();
    delay(100);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}





