#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSansBold12pt7b.h>

// --------- COMMUNICATION BETWEEN ARDUINOS ---------
byte currentElevatorLevel = 0;
void initCommunication(){
  Wire.begin();                           // join i2c bus (address optional for master)
}
void requestMessage(){
  Wire.requestFrom(8, 1);                 // request 1 bytes from peripheral device #8

  while (Wire.available()) {              // peripheral may send less than requested
    byte c = Wire.read();                 // receive a byte as character
    Serial.print("Current elevator level: ");
    Serial.println(c);          
    currentElevatorLevel = c;          
  }
}

// --------- OLED LCD SCREEN --------- 
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (-1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initDisplay(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}

void drawScreen(){
  display.clearDisplay();
  display.setFont(&FreeSansBold12pt7b);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(SCREEN_WIDTH/2-15,SCREEN_HEIGHT/2+15);
  display.print(currentElevatorLevel);
  display.display();
}


// --------- SETUP AND LOOP ---------

void setup() {
  Serial.begin(115200); 
  initCommunication();
  initDisplay();
}

void loop() {
  
  requestMessage();
  drawScreen();
  delay(50);
}
