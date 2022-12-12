//*********************************NOTES*********************************
//A large portion of the clock functions were taken from / inspired
//by Richard Holmes' AOClock (https://github.com/holmesrichards/clock)
//Please visit his repository for a fantastic Clock module 
//*********************************NOTES*********************************

#include <Wire.h>
#include <TimerOne.h>
#include <DirectIO.h>
#include <assert.h>
#include "channels.h" 
#include "ints.h"

void setup() {
  //Serial.begin(9600);

  // Set up OLED 
  oledSetup();
  
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
  u8x8.begin();
  u8x8.clear();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.setCursor(1,2);
  u8x8.print("C1: ");
  u8x8.setCursor(1,3);
  u8x8.print("C2: ");
  u8x8.setCursor(1,4);
  u8x8.print("C3: ");
  u8x8.setCursor(1,5);
  u8x8.print("C4: ");
  u8x8.setCursor(1,6);
  u8x8.print("C5: ");
  u8x8.setCursor(1,7);
  u8x8.print("C6: ");
}

void loop() {
  readButtons();
  static bool started = false;
  if (!started)
    {
      period = (60000000./BPM/PPB);  // period in usec
      ontime = period * duty_cycle * 0.01;
      start_it();
      started = true;
    }
  updateOled();
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
    u8x8.setCursor(0,0);
    u8x8.print("[-]");
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
    u8x8.setCursor(0,0);
    u8x8.print("[+]");
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
        u8x8.setCursor(0,2);
        u8x8.print(" ");
        u8x8.setCursor(0,3);
        u8x8.print("*");
        break;
      case 2:
        currentCh = 3;
        u8x8.setCursor(0,3);
        u8x8.print(" ");
        u8x8.setCursor(0,4);
        u8x8.print("*");
        break;
      case 3:
        currentCh = 4;
        u8x8.setCursor(0,4);
        u8x8.print(" ");
        u8x8.setCursor(0,5);
        u8x8.print("*");
        break;
      case 4:
        currentCh = 5;
        u8x8.setCursor(0,5);
        u8x8.print(" ");
        u8x8.setCursor(0,6);
        u8x8.print("*");
        break;
      case 5:
        currentCh = 6;
        u8x8.setCursor(0,6);
        u8x8.print(" ");
        u8x8.setCursor(0,7);
        u8x8.print("*");
        break;
      case 6:
        currentCh = 1;
        u8x8.setCursor(0,7);
        u8x8.print(" ");
        u8x8.setCursor(0,2);
        u8x8.print("*");
        break;
      default:
        //something random changed mode to an invalid value? Get it back on track.
        currentCh = 1;
        u8x8.setCursor(0,2);
        u8x8.print("*");
        break;
        }
      }
    }
  }

  // save the button reading
  lastButtonState = reading;
}

void updateOled(){
}
