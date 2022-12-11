#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "channels.h"
#include "interface.h" 

LiquidCrystal_I2C lcd(0x27, SCL, SDA);


void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(chBtn, INPUT);
  // setup step LEDs
  for (byte i = 0; i < 8; i++) {
    pinMode(ledArr[i], OUTPUT);
  }

  // setup buttons
  for (byte i = 0; i < 8; i++) {
    pinMode(btnArr[i], INPUT);
  } 

  // setup channel outputs
  for (byte i = 0; i < 6; i++) {
    pinMode(outArr[i], OUTPUT);
  }
}

void loop() {
  displaySet();
  trigMap();
  tempoUI();
}


void tempoUI(){
  for (byte i = 0; i < 8; i++) {
    if (Channel1.stepState[i] == 0) {
      digitalWrite(i, HIGH);
      delay(100);
      digitalWrite(i,LOW);
      delay(5  );
    } else if (Channel1.stepState[i] == 1) {
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
  channelSel();
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

void displaySet(){
  lcd.setCursor(0,0);
  lcd.print("Channel: ");
  lcd.print(currentCh);
  lcd.setCursor(0,1);
  lcd.print("Pattern:");
  for (byte i = 0; i < 8; i++) {
    lcd.print(Channel1.stepState[i]);
  }
}

void channelSel() {
}
