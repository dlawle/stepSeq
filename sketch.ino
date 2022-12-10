#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, SCL, SDA);

// temporary holder for button array
int btnArr[8]={23,25,27,29,31,33,35,37};


//enum channels {c1,c2,c3,c4,c5,c6};
//channels channel; //the current mode must be stored in a global variable

const unsigned long debounceDelay = 50;    // the debounce time milliseconds; increase if the output flickers
int chBtn = 53;
int currentCh = 0;
int step = 0; 

struct {
  enum channels {c1,c2,c3,c4,c5,c6};
  channels channel; //the current mode must be stored in a global variable
  int stepState[8] = {0,0,0,0,0,0,0,0};
  int outArr[6]={41,43,45,47,49,51};
} Channel;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(chBtn, INPUT);
  for (byte i = 0; i < 8; i++) {
    pinMode(i, OUTPUT);
  }
  for (byte i = 0; i < 8; i++) {
    pinMode(btnArr[i], INPUT);
  } 
  for (byte i = 0; i < 6; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  trigMap();
  tempoUI();
}


void tempoUI(){
  for (byte i = 0; i < 8; i++) {
    if (Channel.stepState[i] == 0) {
      digitalWrite(i, HIGH);
      delay(100);
      digitalWrite(i,LOW);
      delay(100);
    } else if (Channel.stepState[i] == 1) {
      digitalWrite(41, HIGH);
      digitalWrite(i, LOW);
      delay(100);
      digitalWrite(i,HIGH);
      digitalWrite(41, LOW);
      delay(100);
    }
  }
}

void trigMap(){
  for (byte i = 0; i < 8;  i++) {
    if (digitalRead(btnArr[i]) == HIGH) {
      digitalWrite(i, HIGH);
      Channel.stepState[i] = 1;
    } else {
      digitalWrite(i, LOW);
      Channel.stepState[i] = 0;
    }
  }
  channelSel();
  displaySet();
}

void displaySet(){
  lcd.setCursor(0,0);
  lcd.print("Channel: ");
  lcd.print(currentCh);
  lcd.setCursor(0,1);
  lcd.print("Pattern:");
  for (byte i = 0; i < 8; i++) {
    lcd.print(Channel.stepState[i]);
  }
}

void channelSel() {
  //read the button(s) in and change modes accordingly
  //currently this example only shows one button
  
  //store some static variables that we need to remember over the long term
  static bool lastButtonState = LOW;   // the previous reading from the input pin
  static bool buttonState = HIGH;     // the current state of the button
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
      switch (Channel.channel) {
        case Channel.c1:
          Channel.channel = Channel.c2;
          currentCh = 2;
          break;
        case Channel.c2:
          Channel.channel = Channel.c3;
          currentCh = 3;
          break;
        case Channel.c3:
          Channel.channel = Channel.c4;
          currentCh = 4;
          break;
        case Channel.c4:
          Channel.channel = Channel.c5;
          currentCh = 5;
          break;
        case Channel.c5:
          Channel.channel = Channel.c6;
          currentCh = 6;
          break;
        case Channel.c6:
          Channel.channel = Channel.c1;
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