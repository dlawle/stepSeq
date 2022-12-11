

void tempoUI(){
  for (byte i = 0; i < 8; i++) {
    if (Channel1.stepState[i] == 0) {
      digitalWrite(ledArr[i], HIGH);
      delay(100);
      digitalWrite(ledArr[i],LOW);
      delay(5  );
    } else if (Channel1.stepState[i] == 1) {
      digitalWrite(ledArr[i], LOW);
      delay(100);
      digitalWrite(ledArr[i],HIGH);
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