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
 
  pinMode(chBtn, INPUT);
  
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
  updateSteps();  
  readButtons(); 

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
  if (ledStp == 0) {
    if (Channel[currentCh][ledStp] == 0) {
      l_1.write(LOW);
    } else if(Channel[currentCh][ledStp] == 1) {
        l_1.write(HIGH);
        }
    } else if (ledStp == 1) {
    if (Channel[currentCh][ledStp] == 0) {
      l_2.write(LOW);
    } else if(Channel[currentCh][ledStp] == 1) {
        l_2.write(HIGH);
        }
      } else if (ledStp == 2) {
    if (Channel[currentCh][ledStp] == 0) {
      l_3.write(LOW);
    } else if(Channel[currentCh][ledStp] == 1) {
        l_3.write(HIGH);
        }
      } else if (ledStp == 3) {
    if (Channel[currentCh][ledStp] == 0) {
      l_4.write(LOW);
    } else if(Channel[currentCh][ledStp] == 1) {
        l_4.write(HIGH);
        }
      } else if (ledStp == 4) {
    if (Channel[currentCh][ledStp] == 0) {
      l_5.write(LOW);
    } else if(Channel[currentCh][ledStp] == 1) {
        l_5.write(HIGH);
        }
      } else if (ledStp == 5) {
    if (Channel[currentCh][ledStp] == 0) {
      l_6.write(LOW);
    } else if(Channel[currentCh][ledStp] == 1) {
        l_6.write(HIGH);
        }
      } else if (ledStp == 6) {
    if (Channel[currentCh][ledStp] == 0) {
      l_7.write(LOW);
    } else if(Channel[currentCh][ledStp] == 1) {
        l_7.write(HIGH);
        }
      } else if (ledStp == 7) {
    if (Channel[currentCh][ledStp] == 0) {
      l_8.write(LOW);
    } else if(Channel[currentCh][ledStp] == 1) {
        l_8.write(HIGH);
        }
  }
    ledStp++;
    if (ledStp == 8){
      ledStp=0;
    }
    display.fillRect(120,0,6,6,SSD1306_WHITE);  
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
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[1]);
            display.print("*");
            break;
          case 2:
            currentCh = 3;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[2]);
            display.print("*");
            
            break;
          case 3:
            currentCh = 4;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[3]);
            display.print("*");
            
            break;
          case 4:
            currentCh = 5;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[4]);
            display.print("*");
            
            break;
          case 5:
            currentCh = 6;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[5]);
            display.print("*");
            
            break;
          case 6:
            currentCh = 1;
            display.fillRect(0,channelYpos[0],6,SCREEN_HEIGHT,SSD1306_BLACK);
            display.setCursor(0,channelYpos[0]);
            display.print("*");
            
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
  display.setCursor(30,0);  
  display.print(BPM);
  trigMap(currentCh -1);
  display.display();
}

void trigMap(int c) {
  if(b_1 == HIGH && l_1 == LOW) {
   // l_1 = HIGH;
   digitalWrite(2,HIGH);
   Channel[c][0] = 1; 
  } else if(b_1 == HIGH && l_1 == HIGH) {
    //l_1 = LOW;
    digitalWrite(2,HIGH);
    Channel[c][0] = 0; 
  }
  if(b_2 == HIGH && l_2 == LOW) {
   l_2 = HIGH;
   Channel[c][1] = 1; 
  } else if(b_2 == HIGH && l_2 == HIGH) {
    l_2 = LOW;
    Channel[c][1] = 0; 
  }
  if(b_3 == HIGH && l_3 == LOW) {
   l_3 = HIGH;
   Channel[c][2] = 1; 
  } else if(b_3 == HIGH && l_3 == HIGH) {
    l_3 = LOW;
    Channel[c][2] = 0; 
  }
  if(b_4 == HIGH && l_4 == LOW) {
   l_4 = HIGH;
   Channel[c][3] = 1; 
  } else if(b_4 == HIGH && l_4 == HIGH) {
    l_4 = LOW;
    Channel[c][3] = 0; 
  }
  if(b_5 == HIGH && l_5 == LOW) {
   l_5 = HIGH;
   Channel[c][4] = 1; 
  } else if(b_5== HIGH && l_5 == HIGH) {
    l_5 = LOW;
    Channel[c][4] = 0; 
  }
  if(b_6 == HIGH && l_6 == LOW) {
   l_6 = HIGH;
   Channel[c][5] = 1; 
  } else if(b_6 == HIGH && l_6 == HIGH) {
    l_6 = LOW;
    Channel[c][5] = 0; 
  }
  if(b_7 == HIGH && l_7 == LOW) {
   l_1 = HIGH;
   Channel[c][6] = 1; 
  } else if(b_7 == HIGH && l_7 == HIGH) {
    l_7 = LOW;
    Channel[c][6] = 0; 
  }
  if(b_8 == HIGH && l_8 == LOW) {
   l_8 = HIGH;
   Channel[c][7] = 1; 
  } else if(b_8 == HIGH && l_8 == HIGH) {
    l_8 = LOW;
    Channel[c][7] = 0; 
  }
}
