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
 

void oledSetup(){
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
}

void timerStuff() {
  // Called by interrupt handler once every 100 us
  // When we pass ontime ticks, call cycle_off
  // When we pass period ticks, call cycle_on
 
  if (running) {
      tickcount += 100; 
      if (clock_state and tickcount - cycle_start_time >= ontime) // should be rollover safe
  {
    endTrig();
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
  if (ledStp == 0) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_1.write(LOW);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_1.write(HIGH);
        
        }
    } else if (ledStp == 1) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_2.write(LOW);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_2.write(HIGH);
        
        }
      } else if (ledStp == 2) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_3.write(LOW);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_3.write(HIGH);
        
        }
      } else if (ledStp == 3) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_4.write(LOW);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_4.write(HIGH);
        
        }
      } else if (ledStp == 4) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_5.write(LOW);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_5.write(HIGH);
        
        }
      } else if (ledStp == 5) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_6.write(LOW);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_6.write(HIGH);
        
        }
      } else if (ledStp == 6) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_7.write(LOW);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_7.write(HIGH);
        
        }
      } else if (ledStp == 7) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_8.write(LOW);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_8.write(HIGH);
        
    }
  }
    ledStp++;
    if (ledStp == 8){
      ledStp=0;
    }
    sendClk(ledStp);
    display.fillRect(120,0,6,6,SSD1306_WHITE);  

}

void step_start() {
  if (ledStp == 0) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_1.write(HIGH);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_1.write(LOW);
        
        }
    } else if (ledStp == 1) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_2.write(HIGH);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_2.write(LOW);
        
        }
      } else if (ledStp == 2) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_3.write(HIGH);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_3.write(LOW);
        
        }
      } else if (ledStp == 3) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_4.write(HIGH);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_4.write(LOW);
        
        }
      } else if (ledStp == 4) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_5.write(HIGH);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_5.write(LOW);
        
        }
      } else if (ledStp == 5) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_6.write(HIGH);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_6.write(LOW);
        
        }
      } else if (ledStp == 6) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_7.write(HIGH);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_7.write(LOW);
        
        }
      } else if (ledStp == 7) {
    if (Channel[currentCh-1][ledStp] == 0) {
      l_8.write(HIGH);
    } else if(Channel[currentCh-1][ledStp] == 1) {
        l_8.write(LOW);
        
    }
  }
  sendTrig(ledStp);
  endClk(ledStp);
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
      if (buttonState == LOW) {
        switch (currentCh) {
          case 1:
            currentCh = 2;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[1]);
            clearLeds();
            display.print("*");
            break;
          case 2:
            currentCh = 3;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[2]);
            clearLeds();
            display.print("*");
            
            break;
          case 3:
            currentCh = 4;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[3]);
            clearLeds();
            display.print("*");
            break;
          case 4:
            currentCh = 5;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[4]);
            clearLeds();
            display.print("*");
            break;
          case 5:
            currentCh = 6;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[5]);
            clearLeds();
            display.print("*");
            break;
          case 6:
            currentCh = 1;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[0]);
            clearLeds();
            display.print("*");
            break;
          default:
            //something random changed mode to an invalid value? Get it back on track.
            currentCh = 1;
            clearLeds();
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
  display.setCursor(6,10);
  display.print("CH1 ");
  display.setCursor(6,19);
  display.print("CH2 ");
  display.setCursor(6,28);
  display.print("CH3 ");
  display.setCursor(6,37);
  display.print("CH4 ");
  display.setCursor(6,46);
  display.print("CH5 ");
  display.setCursor(6,55);
  display.print("CH6 ");
  display.drawRect(120,0,6,6,SSD1306_WHITE); 
  currentCh = 1;
  display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
  display.setCursor(0,channelYpos[0]);
  display.print("*");
  display.display();

}

void updateSteps(){
  int xline = 25;
  display.fillRect(xline,channelYpos[0],SCREEN_WIDTH,SCREEN_HEIGHT,SSD1306_BLACK);
    for (byte i = 0; i < 6; i++) {
      display.setCursor(xline,channelYpos[i]);
      for (byte s = 0; s < 8; s++) {
        display.print(" ");
        display.print(Channel[i][s]);
      }
    }
  incrOled(ledStp);
  display.setCursor(30,0);
  display.fillRect(30,0,50,8,SSD1306_BLACK);
  display.print(BPM);
  trigMap(currentCh -1);
  display.display();
}

void trigMap(int c) {
  int b_1 = digitalRead(23);
  int b_2 = digitalRead(25);
  int b_3 = digitalRead(27);
  int b_4 = digitalRead(29);
  int b_5 = digitalRead(31);
  int b_6 = digitalRead(33);
  int b_7 = digitalRead(35);
  int b_8 = digitalRead(37);
  
if(b_1 == LOW && l_1 == LOW) {
   l_1 = HIGH; 
   Channel[c][0] = 1;
  } else if(b_1 == LOW && l_1 == HIGH) {
    l_1 = LOW; 
    Channel[c][0] = 0;
 }
  if(b_2 == LOW && l_2 == LOW) {
   l_2 = HIGH; 
   Channel[c][1] = 1;
  } else if(b_2 == LOW && l_2 == HIGH) {
    l_2 = LOW; 
    Channel[c][1] = 0;
  }
  if(b_3 == LOW && l_3 == LOW) {
   l_3 = HIGH; 
    Channel[c][2] = 1;
  } else if(b_3 == LOW && l_3 == HIGH) {
    l_3 = LOW; 
    Channel[c][2] = 0;
  }
  if(b_4 == LOW && l_4 == LOW) {
   l_4 = HIGH; 
    Channel[c][3] = 1;
  } else if(b_4 == LOW && l_4 == HIGH) {
    l_4 = LOW; 
    Channel[c][3] = 0;
  }
  if(b_5 == LOW && l_5 == LOW) {
   l_5 = HIGH; 
    Channel[c][4] = 1;
  } else if(b_5== LOW && l_5 == HIGH) {
    l_5 = LOW; 
    Channel[c][4] = 0;
  }
  if(b_6 == LOW && l_6 == LOW) {
   l_6 = HIGH; 
    Channel[c][5] = 1;
  } else if(b_6 == LOW && l_6 == HIGH) {
    l_6 = LOW; 
    Channel[c][5] = 0;
  }
  if(b_7 == LOW && l_7 == LOW) {
   l_1 = HIGH; 
    Channel[c][6] = 1;
  } else if(b_7 == LOW && l_7 == HIGH) {
    l_7 = LOW; 
    Channel[c][6] = 0;
  }
  if(b_8 == LOW && l_8 == LOW) {
   l_8 = HIGH; 
    Channel[c][7] = 1;
  } else if(b_8 == LOW && l_8 == HIGH) {
    l_8 = LOW; 
    Channel[c][7] = 0;
  }
}

void clearLeds(){
  l_1.write(LOW);
  l_2.write(LOW);
  l_3.write(LOW);
  l_4.write(LOW);
  l_5.write(LOW);
  l_6.write(LOW);
  l_7.write(LOW);
  l_8.write(LOW);
}

void sendTrig(int s){
    if(Channel[0][s] == 1){
      digitalWrite(outArr[0], HIGH);
    }
    if(Channel[1][s] == 1){
      digitalWrite(outArr[1], HIGH);
    }
    if(Channel[2][s] == 1){
      digitalWrite(outArr[2], HIGH);
    }
    if(Channel[3][s] == 1){
      digitalWrite(outArr[3], HIGH);
    }
    if(Channel[4][s] == 1){
      digitalWrite(outArr[4], HIGH);
    }
    if(Channel[5][s] == 1){
      digitalWrite(outArr[5], HIGH);
    }
}

void sendClk(int s){
    if (s == 0) {
      digitalWrite(clkOut[0], HIGH);  // 1:1
      digitalWrite(clkOut[1], HIGH);  // 1:2
      digitalWrite(clkOut[2], HIGH);  // 1:4
      digitalWrite(clkOut[3], HIGH);  // 1:8
    } else if (s == 1) {
      digitalWrite(clkOut[0], HIGH);
    } else if (s == 2) {
      digitalWrite(clkOut[0], HIGH);
      digitalWrite(clkOut[1], HIGH);
    } else if (s == 3) {
      digitalWrite(clkOut[0], HIGH);
    } else if (s == 4) {
      digitalWrite(clkOut[0], HIGH);
      digitalWrite(clkOut[1], HIGH);
      digitalWrite(clkOut[2], HIGH);
    } else if (s == 5) {
      digitalWrite(clkOut[0], HIGH);
    } else if (s == 6) {
      digitalWrite(clkOut[0], HIGH);
      digitalWrite(clkOut[1], HIGH);
    } else if (s == 7) {
      digitalWrite(clkOut[0], HIGH);
    }
}

void endClk(int s){
    if (s == 0) {
      digitalWrite(clkOut[0], LOW);  // 1:1
      digitalWrite(clkOut[1], LOW);  // 1:2
      digitalWrite(clkOut[2], LOW);  // 1:4
      digitalWrite(clkOut[3], LOW);  // 1:8
    } else if (s == 1) {
      digitalWrite(clkOut[0], LOW);
    } else if (s == 2) {
      digitalWrite(clkOut[0], LOW);
      digitalWrite(clkOut[1], LOW);
    } else if (s == 3) {
      digitalWrite(clkOut[0], LOW);
    } else if (s == 4) {
      digitalWrite(clkOut[0], LOW);
      digitalWrite(clkOut[1], LOW);
      digitalWrite(clkOut[2], LOW);
    } else if (s == 5) {
      digitalWrite(clkOut[0], LOW);
    } else if (s == 6) {
      digitalWrite(clkOut[0], LOW);
      digitalWrite(clkOut[1], LOW);
    } else if (s == 7) {
      digitalWrite(clkOut[0], LOW);
    }
}

void endTrig(){
  for(byte o = 0; o < 8; o++){
    digitalWrite(outArr[o], LOW);     
  }
}

void bpmSet(){
  float tempoUpdate = analogRead(tempo); 
  BPM = map(tempoUpdate, 0, 1023, min_BPM, max_BPM);
  Serial.println(BPM);
}

void incrOled(int s) {
  switch (s) {
          case 0:
              display.drawRect(28,channelYpos[0]-2,10,64,SSD1306_WHITE);
              break;

          case 1:
              display.drawRect(40,channelYpos[0]-2,10,64,SSD1306_WHITE);
              break;

          case 2:
              display.drawRect(52,channelYpos[0]-2,10,64,SSD1306_WHITE);
              break;

          case 3:
              display.drawRect(64,channelYpos[0]-2,10,64,SSD1306_WHITE);
              break;

          case 4:
              display.drawRect(76,channelYpos[0]-2,10,64,SSD1306_WHITE);
              break;

          case 5:
              display.drawRect(88,channelYpos[0]-2,10,64,SSD1306_WHITE);
              break;

          case 6:
              display.drawRect(100,channelYpos[0]-2,10,64,SSD1306_WHITE);
              break;

          case 7:
              display.drawRect(112,channelYpos[0]-2,10,64,SSD1306_WHITE);
              break;
  }
  display.display();
}

void setup() {
  Serial.begin(9600);

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
  
  // setup channel outputs
  for (byte i = 0; i < 6; i++) {
    pinMode(outArr[i], OUTPUT);
  }

  // setup clock outputs 
  for (byte i = 0; i < 4; i++) {
    pinMode(clkOut[i], OUTPUT);
  }  

  // setup internal pullup buttons:
  pinMode(23,INPUT_PULLUP);
  pinMode(25,INPUT_PULLUP);
  pinMode(27,INPUT_PULLUP);
  pinMode(29,INPUT_PULLUP);
  pinMode(31,INPUT_PULLUP);
  pinMode(33,INPUT_PULLUP);
  pinMode(35,INPUT_PULLUP);
  pinMode(37,INPUT_PULLUP);
  pinMode(chBtn, INPUT_PULLUP);
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
  bpmSet(); 
  updateSteps();  
  readButtons(); 
  period = (60000000./BPM/PPB);  // period in usec
  ontime = period * duty_cycle * 0.01;
}
