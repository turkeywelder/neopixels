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
byte _outer[] = {
  0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
byte _middle[] = { 
  24,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25};
byte _inner[] = {
  41,42,43,44,45,46,40};

size_t _innerSize = sizeof(_inner);
size_t _middleSize = sizeof(_middle);
size_t _outerSize = sizeof(_outer);


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(12);
}

void loop() {
  strip.setBrightness(12);
  pulse(255,115,0);
    pulse(255,10,10);
      pulse(50,55,255);
        pulse(0,255,50);
  //  rainbowBar(true);
  //  rainbowBar(false);
  //  radar();
  //  radar();
  //  strip.clear();
  //  outerLoop();
  //  middleLoop();
  //  innerLoop();
  //  //
  //  barSpiral(false);
  //  barSpiral(true);
  //
    for(byte i=0; i<9; i++)
    {
      fireWork(Wheel((int)random(0, 255)));
   }
}

void pulse(byte r, byte g, byte b)
{
  strip.clear();
  strip.show();
  //in
  fadeAll(true, _inner, _innerSize, r,g,b);
  fadeAll(false, _inner, _innerSize, r,g,b);

  fadeAll(true, _middle, _middleSize, r,g,b);
  fadeAll(false, _middle, _middleSize, r,g,b);
  
  fadeAll(true, _outer, _outerSize, r,g,b);
fadeAll(false, _outer, _outerSize, r,g,b);



  

}


void fadeIn(byte addresses[], size_t arrSize, byte red, byte green, byte blue){
  for(byte i=0; i<arrSize; i++){
    for(byte j=0; j<255; j++){

      byte xr = map(j, 0, 255, 0, red);
      byte xg = map(j, 0, 255, 0, green);
      byte xb = map(j, 0, 255, 0, blue);
      uint32_t faded = strip.Color(xr-255, xg-255, xb-255);
      strip.setPixelColor(addresses[i], faded);
      delay(1);
      strip.show();
    }
  }
}

void fadeAll(boolean in, byte addresses[], size_t arrSize, byte red, byte green, byte blue){
  for(byte j=0; j<255; j++){
    byte xr = 0; 
    byte xg = 0; 
    byte xb = 0;
    uint32_t faded = 0;

    if(in){
      xr = map(j, 0, 255, 0, red);
      xg  = map(j, 0, 255, 0, green);
      xb =  map(j, 0, 255, 0, blue);
      faded    = strip.Color(xr-255, xg-255, xb-255);
    }
    else{
      xr = map(255-j, 0, 255, 0, red);
      xg = map(255-j, 0,255, 0, green);
      xb = map(255-j, 0,255, 0, blue);
      faded = strip.Color(xr, xg, xb);
    }

    for(byte i=0; i<arrSize; i++){
      strip.setPixelColor(addresses[i], faded);
    }
    strip.show();
  }
}

void rainbowBar(boolean stack){
  strip.clear();
  uint32_t colour = 0; 
  for (int i = 0; i < 24; i++) {
    if(!stack){
      strip.clear();
    }
    colour = Wheel(i*5);

    byte midIndex = map(i, 0, 23, 0, 15);
    byte innIndex = map(i, 0, 23, 0, 6); 

    //40 is the centre point, always on:
    //full intensity:
    strip.setPixelColor(40, colour);
    strip.setPixelColor(_outer[i], colour);
    strip.setPixelColor(_middle[midIndex], colour);
    strip.setPixelColor(_inner[innIndex], colour);

    delay(60);
    strip.show();
  }
}

// All three rings circle and highlight a random pixel on each loop
byte blip = 0;
byte fadecount = 0;

void radar(){
  randomSeed(analogRead(0));
  byte midIndex = 0;
  byte innIndex = 0;
  int r = 70; //strip.Color(20, 190,40);
  int g = 10;
  int b = 10;
  uint32_t red = strip.Color(0, 255, 0);

  for (int i = 0; i < 24; i++) {

    strip.clear();
    if(fadecount > 0)
    {
      fadecount++;
      int fade = map(fadecount, 0, 23, 50, 250);
      strip.setPixelColor(blip, strip.Color(0, 255-fade, 0));
      if(fade > 240){
        fadecount = 0;
        blip = (int)random(0, NUMPIXELS);
      }
    }

    midIndex = map(i, 0, 23, 0, 15);
    innIndex = map(i, 0, 23, 0, 6); 

    //40 is the centre point, always on:
    //full intensity:
    strip.setPixelColor(40, r,g,b);
    strip.setPixelColor(_outer[i], r,g,b);
    strip.setPixelColor(_middle[midIndex], r,g,b);
    strip.setPixelColor(_inner[innIndex], r,g,b);

    //turn the blip on if it's on this bar  
    if(fadecount == 0 && 
      (blip == _outer[i] ||
      blip == _middle[midIndex] || 
      blip == _inner[innIndex])){
      strip.setPixelColor(blip, red);
      fadecount = 1;
    }

    strip.show();
    delay(70);
  }


}

void fireWork(uint32_t color){
  // run up the middle,
  // to the centre and flash rings outwards, 
  // WITH A FADEOUT!

  uint32_t faded = color / 2;
//  uint32_t orange = strip.Color(180, 120,1);
  int vel = 100;

  strip.setPixelColor(12, color);
  delay(vel);
  strip.show();  
  strip.setPixelColor(32, color);
  delay(vel);
  strip.show();  
  strip.setPixelColor(44, color);
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
    {
      0, 24, 41, 40, 44, 32, 12                                            }
    ,{
      1, 24, 41, 40, 44, 31, 13                                            }
    ,{
      2, 39, 41, 40, 44, 31, 14                                            }
    ,{
      3, 38, 42, 40, 45, 30, 15                                            }
    ,{
      4, 38, 42, 40, 45, 30, 16                                            }
    ,{
      5, 37, 42, 40, 45, 29, 17                                            }
    ,{
      6, 36, 43, 40, 45, 29, 18                                            }
    ,{
      7, 35, 43, 40, 46, 28, 19                                            }
    ,{
      8, 34, 43, 40, 46, 27, 20                                            }
    ,{
      9, 34, 43, 40, 46, 26, 21                                            }
    ,{
      10, 33, 44, 40, 41, 26, 22                                            }
    ,{
      11, 33, 44, 40, 41,25, 23                                            }
  };

  byte c = 0;

  for (byte i = 0; i < 13; i++) {
    //new line, clear the old one and pause:
    if(!leaveOn){
      strip.clear();
    }

    for (byte j = 0; j < 7; j++) {
      strip.setPixelColor(steps[i][j], Wheel(c*2));  
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
















