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
const int resetbuttonPin = 13;     // the ID of the reset pushbutton pin

int ledPins[] = {4, 5, 6, 7};
int numberOfLED = 4;

// Variables will change:
int secbuttonPushCounter = 0;   // counter for the number of button presses
int minbuttonPushCounter = 0;   // counter for the number of button presses
int minbuttonState = 0;         // current state of the button
int lastminButtonState = 0;     // previous state of the button
int secbuttonState = 0;         // current state of the button
int lastsecButtonState = 0;     // previous state of the button
int resetbuttonState = 0;

void setup()
   {
   for (int i = 0; i < numberOfLED; i++)
      {  
      pinMode(ledPins[i], OUTPUT);  
      } 
  
   // initialize the button pin as a input:
    
   pinMode(minbuttonPin, INPUT);
   pinMode(secbuttonPin, INPUT);
   Serial.begin(9600);
   }

void loop() 
   {
   display.setBrightness(0x0f);
  
   // read the pushbutton input pin:
   secbuttonState = digitalRead(secbuttonPin);
   minbuttonState = digitalRead(minbuttonPin);

   // compare the buttonState to its previous state
   if (secbuttonState != lastsecButtonState)
   {
      // if the state has changed, increment the counter
      if (secbuttonState == HIGH)  
      {
         // if the current state is HIGH then the button went from off to on:
         count = 40;
         if (secbuttonPushCounter < 4)
         {
            secbuttonPushCounter++;
         }
         if(minbuttonPushCounter != 0) //if we had a 2 minute set on the docket
           {
           for (int i = 0; i <= 4; i++)
            {
               digitalWrite(ledPins[i], LOW);
            }
            minbuttonPushCounter = 0; //expunge its count
            }
         
         Serial.print("number of second button pushes:  ");
         Serial.println(secbuttonPushCounter);
            for (int i = 0; i < secbuttonPushCounter; i++)
            {
               // turn LED on:  
               digitalWrite(ledPins[i], HIGH);
            }      
   }
   }       
   else if (minbuttonState != lastminButtonState)
   {
      if (minbuttonState == HIGH)  
      {
         // if the current state is HIGH then the button went from off to on:
         count = 120; //redefine counter as 2 mins
         if (minbuttonPushCounter < 4) //check to see sets < 4
         {
            minbuttonPushCounter++; //add a set
         }
         if(secbuttonPushCounter != 0) //if we had a 40 second set on the docket
         {
         for (int i = 0; i <= 4; i++)
            {
               digitalWrite(ledPins[i], LOW);
            }
         secbuttonPushCounter = 0; //expunge its count
         }
         Serial.print("number of minute button pushes:  ");
         Serial.println(minbuttonPushCounter);
         for (int i = 0; i < minbuttonPushCounter; i++)
            {
            // turn LED on:  
            digitalWrite(ledPins[i], HIGH);
            }
      }
   }
   // Delay a little bit to avoid bouncing
         delay(50);
      
  
         // save the current state as the last state, for next time through the loop
   lastminButtonState = minbuttonState;
   lastsecButtonState = secbuttonState;
         
   data[3] = display.encodeDigit(count/1 % 10);
   data[2] = display.encodeDigit(count%60 / 10);
   data[1] = display.encodeDigit(count/60 % 10);
   data[0] = display.encodeDigit(count/600);
   display.setSegments(data);

   unsigned long currentMillis = millis();
 
   if(currentMillis - previousMillis >= interval)
   {
      previousMillis = currentMillis;   
      if(count > 0) 
      {
      count--;
      }
   } 
resetbuttonState = digitalRead(resetbuttonPin);
if (resetbuttonState == HIGH)  
   {
   count = 0;
   minbuttonPushCounter = 0;
   secbuttonPushCounter = 0;
   for (int i = 0; i <= 4; i++)
      {
      digitalWrite(ledPins[i], LOW);
      }
   }
}
