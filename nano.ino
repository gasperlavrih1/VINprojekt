#include <Wire.h>

// --------- 7 SEGMENT DISPLAY ---------
#define A 8
#define B 9
#define C 12
#define D 11
#define E 10
#define F 7
#define G 6
int seg[] {A, B, C, D, E, F, G};
byte Chars[4][7] {
  {0, 1, 1, 0, 0, 0, 0}, //1
  {1, 1, 0, 1, 1, 0, 1}, //2
  {1, 1, 1, 1, 0, 0, 1}, //3
  {0, 1, 1, 0, 0, 1, 1}, //4
};
void printCharacter(byte id) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(seg[i], LOW);    // clear old character
    if (Chars[id-1][i] == 1) {
      digitalWrite(seg[i], HIGH); // write new character
    }
  }
}
void init7segment(){
  pinMode(seg[0], OUTPUT);
  pinMode(seg[1], OUTPUT);
  pinMode(seg[2], OUTPUT);
  pinMode(seg[3], OUTPUT);
  pinMode(seg[4], OUTPUT);
  pinMode(seg[5], OUTPUT);
  pinMode(seg[6], OUTPUT);
}

// --------- BUTTONS ---------
#define Button1 2
#define Button2 3
#define Button3 4
#define Button4 5
void initButtons() {
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);
  pinMode(Button4, INPUT);
}
bool isButtonPressed(byte buttonID) {
  return digitalRead(buttonID + 1); // because button1 = 2, button2 = 3, ...
}
byte anyButtonPressed(){
  for(int i = 1; i <= 4; i++){
    if (isButtonPressed(i)){
      return i;
    }
  }
  return 0;
}

// --------- ELEVATOR LOGIC ---------
byte currentElevatorLevel = 1;
void callElevator(byte toLevel){
  
  if (currentElevatorLevel == toLevel) return;  // dont do anything if were already on that level

  while(currentElevatorLevel != toLevel){       // keep going until we get to the desired level
   
    if (currentElevatorLevel < toLevel){        // if were going up
      currentElevatorLevel++;
   
    }else{                                      // if were going down
      currentElevatorLevel--;
    }
    Serial.print("Current level: ");
    Serial.println(currentElevatorLevel);
  
    printCharacter(currentElevatorLevel);  
    delay(1000);
  }
  
}

// --------- COMMUNICATION BETWEEN ARDUINOS ---------
void initCommunication() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}
void requestEvent() {
  Wire.write(currentElevatorLevel);
}

// --------- SETUP AND LOOP ---------
void setup() {
  Serial.begin(115200);

  initButtons();
  init7segment();
  initCommunication();
  printCharacter(currentElevatorLevel);

}

void loop() {
  
  byte pressedButton = anyButtonPressed();    // check if any button is pressed
  if(pressedButton){                          // if so, then call the elevator
    delay(1000);
    callElevator(pressedButton);
  }
}
