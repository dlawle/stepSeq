//*********************************NOTES*********************************
//A large portion of the clock functions were taken from / inspired
//by Richard Holmes' AOClock (https://github.com/holmesrichards/clock)
//Please visit his repository for a fantastic Clock module 
//*********************************NOTES*********************************

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimerOne.h>
#include <DirectIO.h>
#include <assert.h>
#include "channels.h" 
#include "ints.h"

void setup() {
  //Serial.begin(9600);
  lcd.init();
  lcd.backlight();

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
  // start on channel one for LCD 
  currentCh = 1;
  lcd.setCursor(0,0);
  lcd.print("Channel:");
  lcd.print(currentCh);
  lcd.setCursor(11,0);
  lcd.print(BPM);
  lcd.setCursor(0,1);
  lcd.print("Pattern:");
  for (byte i = 0; i < 8; i++) {
    lcd.print(Channel1.stepState[i]);
  }
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
        lcd.setCursor(0,0);
        lcd.print("Channel:");
        lcd.print(currentCh);
        lcd.setCursor(11,0);      
        lcd.print(BPM);
        lcd.setCursor(0,1);
        lcd.print("Pattern:");
        for (byte i = 0; i < 8; i++) {
          lcd.print(Channel2.stepState[i]);
        }
        break;
      case 2:
        currentCh = 3;
        lcd.setCursor(0,0);
        lcd.print("Channel:");
        lcd.print(currentCh);
        lcd.setCursor(11,0);
        lcd.print(BPM);
        lcd.setCursor(0,1);
        lcd.print("Pattern:");
        for (byte i = 0; i < 8; i++) {
          lcd.print(Channel3.stepState[i]);
        }
        break;
      case 3:
        currentCh = 4;
        lcd.setCursor(0,0);
        lcd.print("Channel:");
        lcd.print(currentCh);
        lcd.setCursor(11,0);
        lcd.print(BPM);
        lcd.setCursor(0,1);
        lcd.print("Pattern:");
        for (byte i = 0; i < 8; i++) {
          lcd.print(Channel4.stepState[i]);
        }
        break;
      case 4:
        currentCh = 5;
        lcd.setCursor(0,0);
        lcd.print("Channel:");
        lcd.print(currentCh);
        lcd.setCursor(11,0);
        lcd.print(BPM);
        lcd.setCursor(0,1);
        lcd.print("Pattern:");
        for (byte i = 0; i < 8; i++) {
          lcd.print(Channel5.stepState[i]);
        }
        break;
      case 5:
        currentCh = 6;
        lcd.setCursor(0,0);
        lcd.print("Channel:");
        lcd.print(currentCh);
        lcd.setCursor(11,0);
        lcd.print(BPM);
        lcd.setCursor(0,1);
        lcd.print("Pattern:");
        for (byte i = 0; i < 8; i++) {
          lcd.print(Channel6.stepState[i]);
        }
        break;
      case 6:
        currentCh = 1;
        lcd.setCursor(0,0);
        lcd.print("Channel:");
        lcd.print(currentCh);
        lcd.setCursor(11,0);
        lcd.print(BPM);
        lcd.setCursor(0,1);
        lcd.print("Pattern:");
        for (byte i = 0; i < 8; i++) {
          lcd.print(Channel1.stepState[i]);
        }
        break;
      default:
        //something random changed mode to an invalid value? Get it back on track.
        currentCh = 1;
        lcd.setCursor(0,0);
        lcd.print("Channel:");
        lcd.print(currentCh);
        lcd.setCursor(11,0);
        lcd.print(BPM);
        lcd.setCursor(0,1);
        lcd.print("Pattern:");
        for (byte i = 0; i < 8; i++) {
          lcd.print(Channel1.stepState[i]);
        }
        break;
        }
      }
    }
  }

  // save the button reading
  lastButtonState = reading;
}

void trigMap(){
  for (byte i = 0; i < 8;  i++) {
    if (digitalRead(btnArr[i]) == HIGH) {
      digitalWrite(i, HIGH);
      Channel1.stepState[i] = 1;
    } else {
      digitalWrite(i, LOW);
      Channel1.stepState[i] = 0;
    }
  }
}
