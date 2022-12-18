// contains struct information for each channel. 
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
