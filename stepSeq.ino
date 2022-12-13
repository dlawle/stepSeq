//*********************************NOTES*********************************
//A large portion of the clock functions were taken from / inspired
//by Richard Holmes' AOClock (https://github.com/holmesrichards/clock)
//Please visit his repository for a fantastic Clock module 
//*********************************NOTES*********************************
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <TimerOne.h>
#include <DirectIO.h>
#include <assert.h>
#include "channels.h" 
#include "ints.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI  11
#define OLED_CLK   13
#define OLED_DC    9
#define OLED_CS    10
#define OLED_RESET 8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
  
void setup() {
  //Serial.begin(9600);

  //Set Up Oled
   display.begin(SSD1306_SWITCHCAPVCC);
   display.clearDisplay();  
   setDisplay();
  // Timer interrupt
  running = true;
  Timer1.initialize(100); // interrupt every 1 ms
  Timer1.attachInterrupt(timerStuff);

  // set LED step array to the first step
  ledStp = 0;
  
  // DirectIO Mapping
  pinMode (l_1, OUTPUT);
  pinMode (l_2, OUTPUT);
  pinMode (l_3, OUTPUT);
  pinMode (l_4, OUTPUT);
  pinMode (l_5, OUTPUT);
  pinMode (l_6, OUTPUT);
  pinMode (l_7, OUTPUT);    
  pinMode (l_8, OUTPUT);

  pinMode (b_1, INPUT);
  pinMode (b_2, INPUT);
  pinMode (b_3, INPUT);
  pinMode (b_4, INPUT);
  pinMode (b_5, INPUT);
  pinMode (b_6, INPUT);
  pinMode (b_7, INPUT);    
  pinMode (b_8, INPUT);
  pinMode (chBtn, INPUT);
  
  // setup channel outputs
  for (byte i = 0; i < 6; i++) {
    pinMode(outArr[i], OUTPUT);
  }
}

void oledSetup(){
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  
}

void loop() {
  static bool started = false;
  if (!started)
    {
      period = (60000000./BPM/PPB);  // period in usec
      ontime = period * duty_cycle * 0.01;
      start_it();
      started = true;
    }
  readButtons();
  updateSteps();
  display.display();
}

void timerStuff() {
  // Called by interrupt handler once every 100 us
  // When we pass ontime ticks, call cycle_off
  // When we pass period ticks, call cycle_on
  
  if (running) {
      tickcount += 100; 
      if (clock_state and tickcount - cycle_start_time >= ontime) // should be rollover safe
  {
    step_end();
    clock_state = false;
  }
      if (tickcount - cycle_start_time >= period)
  {
    step_start();
    clock_state = true;
    cycle_start_time += period;
  }
    }
}

void step_end() {
  // Turn off LEDs and increment counters
  if (running) {
    l_1.write(LOW);
    l_2.write(LOW);
    l_3.write(LOW);
    l_4.write(LOW);
    l_5.write(LOW);
    l_6.write(LOW);
    l_7.write(LOW);
    l_8.write(LOW);
    ledStp++;
    if (ledStp == 8){
      ledStp=0;
    }
    display.fillRect(120,0,6,6,SSD1306_WHITE);
  }  
}

void step_start() {
      if (ledStp == 0) {
        l_1.write(HIGH);
      } else if (ledStp == 1) {
        l_2.write(HIGH);
      } else if (ledStp == 2) {
        l_3.write(HIGH);
      } else if (ledStp == 3) {
        l_4.write(HIGH);
      } else if (ledStp == 4) {
        l_5.write(HIGH);
      } else if (ledStp == 5) {
        l_6.write(HIGH);
      } else if (ledStp == 6) {
        l_7.write(HIGH);
      } else if (ledStp == 7) {
        l_8.write(HIGH);
  }
  display.fillRect(121,1,4,4,SSD1306_BLACK);
}

void start_it() {
  step_start();
  clock_state = true;
  cycle_start_time = tickcount;
}

void readButtons() {
  //read the button(s) in and change modes accordingly
  //currently this example only shows one button
  
  //store some static variables that we need to remember over the long term
  static bool lastButtonState = HIGH;   // the previous reading from the input pin
  static bool buttonState = LOW;     // the current state of the button
  static unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled

  // read the state of the switch into a local variable:
  bool reading = digitalRead(chBtn);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      //Only change mode if the new button state is LOW (pressed)
      //depending on which mode we are in, we may jump to a different one
      if (buttonState == HIGH) {
    switch (currentCh) {
      case 1:
        currentCh = 2;
        break;
      case 2:
        currentCh = 3;
        break;
      case 3:
        currentCh = 4;
        break;
      case 4:
        currentCh = 5;
        break;
      case 5:
        currentCh = 6;
        break;
      case 6:
        currentCh = 1;
        break;
      default:
        //something random changed mode to an invalid value? Get it back on track.
        currentCh = 1;
        break;
        }
      }
    }
  }

  // save the button reading
  lastButtonState = reading;
}

void setDisplay(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("BPM ");
  display.drawFastHLine(0, 8,128,SSD1306_WHITE);
  display.drawFastVLine(24,0,64,SSD1306_WHITE);
  display.setCursor(4,10);
  display.print("CH1 ");
  display.setCursor(4,19);
  display.print("CH2 ");
  display.setCursor(4,28);
  display.print("CH3 ");
  display.setCursor(4,37);
  display.print("CH4 ");
  display.setCursor(4,46);
  display.print("CH5 ");
  display.setCursor(4,55);
  display.print("CH6 ");
  display.drawRect(120,0,6,6,SSD1306_WHITE); 

  display.display();

}

void updateSteps(){
  int xline = 25;
    for (byte i = 0; i < 6; i++) {
      display.setCursor(xline,channelYpos[i]);
      for (byte s = 0; s < 8; s++) {
        display.print(" ");
        display.print(Channel[i][s]);
      }
    }
  display.setCursor(30,0);  
  display.print(BPM);
}
