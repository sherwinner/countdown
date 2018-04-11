//display stuff
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 11
#define DIO 12

TM1637Display display(CLK, DIO);

int count = 0; // initial countdown start value in seconds
long previousMillis = 0;
long interval = 1000;
uint8_t data[] = {0, 0, 0, 0};

const int minbuttonPin = 2;     // the ID of the 2 minute pushbutton pin
const int secbuttonPin = 3;     // the ID of the 40 sec pushbutton pin

// Variables will change:
int secbuttonPushCounter = 0;   // counter for the number of button presses
int minbuttonPushCounter = 0;   // counter for the number of button presses
int minbuttonState = 0;         // current state of the button
int lastminButtonState = 0;     // previous state of the button
int secbuttonState = 0;         // current state of the button
int lastsecButtonState = 0;     // previous state of the button

void setup() {
  // initialize the button pin as a input:
  pinMode(minbuttonPin, INPUT);
  pinMode(secbuttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
   display.setBrightness(0x0f);
  
   // read the pushbutton input pin:
   secbuttonState = digitalRead(secbuttonPin);
   minbuttonState = digitalRead(minbuttonPin);

   // compare the buttonState to its previous state
   if (secbuttonState != lastsecButtonState){
      // if the state has changed, increment the counter
      if (secbuttonState == HIGH)  {
      // if the current state is HIGH then the button
      // wend from off to on:
      minbuttonPushCounter = 0;
      if (secbuttonPushCounter < 4){
         secbuttonPushCounter++;
      }
      count = 40;
      Serial.println("second button on");
      Serial.print("number second button pushes:  ");
      Serial.println(secbuttonPushCounter);
//      secbuttonPushCounter = thisPin;
    }
   }
    else if (minbuttonState != lastminButtonState){
       // if the state has changed, increment the counter
       if (minbuttonState == HIGH)  {
       // if the current state is HIGH then the button
       // wend from off to on:
       count = 120;
       if (minbuttonPushCounter < 4){
         minbuttonPushCounter++;
       }
       secbuttonPushCounter = 0;
       Serial.println("on");
       Serial.print("number of minute button pushes:  ");
      Serial.println(minbuttonPushCounter);
//      minbuttonPushCounter = thisPin;
       }
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  
  // save the current state as the last state,
  //for next time through the loop
lastminButtonState = minbuttonState;
lastsecButtonState = secbuttonState;

data[3] = display.encodeDigit(count/1 % 10);
  data[2] = display.encodeDigit(count%60 / 10);
  data[1] = display.encodeDigit(count/60 % 10);
  data[0] = display.encodeDigit(count/600);
  display.setSegments(data);

  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
     previousMillis = currentMillis;   
     if(count > 0) {
        count--;
     }   
  }
}
