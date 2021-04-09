volatile int flow_frequency; // Measures flow sensor pulses
 float vol = 0.0,l_minute=0;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
const int motorPin1  = 5;  // Pin 14 of L293
const int motorPin2  = 6;  // Pin 10 of L293
int threshold=25;
int motorspeed=125;
int potpin=A0;
float error=0;//PID CONTROL
int motorspeedadjust=0;
void flow () // Interrupt function
{
   flow_frequency++;
}
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup()
{
    Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Water Flow Sensor");
  display.display(); 
  delay(1000);

  

pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
   pinMode(flowsensor, INPUT);
    digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   currentTime = millis();
   cloopTime = currentTime;
   delay(3000);
   flow_frequency=0;
}
void loop ()
{
int potthreshold=analogRead(potpin);
threshold=map(potthreshold,1,1023,0,50);

  motorspeed=125;
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   { display.clearDisplay();
          display.setCursor(0, 10);
             display.print("Desired FLow: ");
      display.print(threshold);
      display.println(" L/M");
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7Q, Q is flow rate in L/min.
       
      //display.println(flow_frequency);
      l_minute = (flow_frequency / 7); // (Pulse frequency x 60 min) / 98Q = flowrate in L/hour
      display.print("Rate: ");
      display.print(l_minute);
      display.println(" L/M");
     // l_minute = l_minute/60; //
      vol = vol +l_minute;
      display.print("Vol:");
      display.print(vol);
      display.println(" L");
      flow_frequency = 0; // Reset Counter
      //display.print(l_minute, DEC); // Print litres/sec
      //display.println(" L/Sec");

        
      error=(threshold-l_minute);
       
      motorspeedadjust=0.04*error*125;
     

      motorspeed=motorspeed+motorspeedadjust;
                  display.print("Motorspeed: ");
                  if(motorspeed>255)
                  {
                    motorspeed=255;
                  }
                  if (motorspeed<0)
                  {
                    motorspeed=0;
                  }
                    analogWrite(motorPin1, motorspeed);
  analogWrite(motorPin2, 0);
      display.print(motorspeed);             
    
      display.display(); 
         }
    else {
      display.println(" flow rate = 0 ");
      display.setCursor(0,0);
      display.print("Rate: ");
      display.print( flow_frequency );
      display.print(" L/M");
      display.setCursor(0,1);
      display.print("Vol:");
      display.print(vol);
      display.print(" L");
    }
}
}
