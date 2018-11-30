#include <Adafruit_NeoPixel.h>
#define PIN 10
#define NUMLIGHTS 240
#define NUMSTRIP 60

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLIGHTS, PIN, NEO_GRB + NEO_KHZ800); 
//fixed settings 
int analogPinL = 1; // read from multiplexer using analog input 0
int analogPinR = 0; // read from multiplexer using analog input 0
int strobePin = 4; // strobe is attached to digital pin 4
int resetPin = 5; // reset is attached to digital pin 5
int spectrumValueL[7];
int spectrumValueR[7]; // to hold a2d values
int start(0);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // print to serial monitor
  Serial.print("hello");
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(analogPinL, INPUT);
  pinMode(analogPinR, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);
  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH); 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  int maxi, maxv(0);
  
  
  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValueL[i] = analogRead(analogPinL);
    spectrumValueR[i] = analogRead(analogPinR);
    //remove noise
    if (spectrumValueL[i] < 100) {
      spectrumValueL[i] = 0;
    }
    if (spectrumValueR[i] < 100) {
      spectrumValueR[i] = 0;
    }
    //find max frequency and band
    if (spectrumValueL[i] > maxv) {
      maxv = spectrumValueL[i];
      maxi = i;
    }
    if (spectrumValueR[i] > maxv) {
      maxv = spectrumValueR[i];
      maxi = i;
    }
    digitalWrite(strobePin, HIGH); 
  } //for i
  //output
    String maxOut = "MAX: " + String(maxi) + " " + String(maxv);
    //Serial.println(maxOut); 
  int r,g,b;
  int moreR, moreO, moreY, moreG, moreB, moreI, moreV;
  switch (maxi) {
    case 1: //R
      moreO = max(spectrumValueR[2],spectrumValueL[2]);
      r = round((maxv/1023)*255);
      g = 0;
      b = 0;
//      if (moreO > 500) {
//        g = round((moreO/1023)*200);
//      }
      break; 
     case 2: //O
      moreR = max(spectrumValueR[1],spectrumValueL[1]);
      moreY = max(spectrumValueR[3],spectrumValueL[3]);
      r = round((maxv/1023)*255);
      g = round((maxv/1023)*180);
      b = 0; 
//      if(moreR>moreY && moreR > 500) {
//          g = round((moreR/1023)*100);
//      } else if (moreY>moreR && moreY>500){ 
//          g = round((moreY/1023)*255);
//      } 
      break;
     case 3: //Y
      moreO = max(spectrumValueR[2],spectrumValueL[2]);
      moreG = max(spectrumValueR[4],spectrumValueL[4]);
      r = round((maxv/1023)*255);
      g = round((maxv/1023)*255);
      b = 0;
//      if (moreO > moreG && moreO > 500) {
//        g = round((3*((maxv/1023)*255)+((moreO/1023)*255))/4);
//      } else if(moreG > moreO && moreG > 500) {
//        g = round((maxv/1023)*255);
//        b = round((moreG/1023)*255);
//      }
      break;
     case 4: //G
      moreY = max(spectrumValueR[3],spectrumValueL[3]);
      moreB = max(spectrumValueR[5],spectrumValueL[5]);
      g = round((maxv/1023)*255);
      r = 0;
      b = 0;
//      if (moreY > moreB && moreY>500) {
//        r = round((moreY/1023)*200);
//      } else if (moreB > moreY && moreB > 500) {
//        b = round((moreB/1023)*200);
//      }
      break;
     case 5: //B
      moreG = max(spectrumValueR[4],spectrumValueL[4]);
      moreI = max(spectrumValueR[6],spectrumValueL[6]);
      b = round((maxv/1023)*255);
      r = 0;
      g = 0;
//      if (moreG > moreI && moreG > 500) {
//        g = round((moreG/1023)*200);
//      } if (moreI > moreG && moreI > 500) {
//        r = round((moreI/1023)*200);
//      }
      break;
     case 6: //I
      moreB = max(spectrumValueR[5],spectrumValueL[5]);
      moreV = max(spectrumValueR[7],spectrumValueL[7]);
      b = round((maxv/1023)*255);
      r = round((maxv/1023)*150);
      g = 0;
//      if(moreB>moreV && moreB > 500) {
//        r = round((((moreB/1023)*180)+3*r)/4);
//      } else if (moreV>moreB && moreV > 500) {
//        g = round((moreV/1023)*150);
//        r = round((((moreV/1023)*180)+3*r)/4);
//      }
      break;
     case 0:
       moreI = max(spectrumValueR[6],spectrumValueL[6]);
       r = round((maxv/1023)*255);
       g = round((maxv/1023)*150);
       b = round((maxv/1023)*255);
//       if (moreI > 500) {
//        r = round((3*r + round((moreI/1023)*255))/4);
//        g = round((2*r + round((moreI/1023)*255))/3);
//       }
       break;
      default:
        r = -1;
        g = -1;
        b = -1;
        break;
  } // switch
  // Serial.println(maxv);

  for (int k = start; k < (start+NUMSTRIP); k++) {
    
    String color =  String(r) + " " + String(g) + " " + String(b);
   // Serial.println(color);
    strip.setPixelColor( k, r, g, b);
  } // for k 
  if (start >= 180) {
    start = 0;
  } else { 
    start += NUMSTRIP;
    
  }
  strip.show();
}
