#include "SevSeg.h"
SevSeg sevseg; 

const int aPoint = A0;
const int rButton = A1;
const int bPoint = A2;

int homePoint;
int resetState;
int awayPoint;

int homeScore = 0;
int awayScore = 0;
int currentScore = 0;

int currentButtonState = 1;
int lastButtonState = 0;

void setup() {

  Serial.begin(9600);
  pinMode(aPoint, INPUT_PULLUP);
  pinMode(rButton, INPUT_PULLUP);
  pinMode(bPoint, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  byte numDigits = 4;  
  byte digitPins[] = {2, 3, 4, 5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
  bool resistorsOnSegments = 0; 
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
  sevseg.setNumber(currentScore, 2);
}

void loop() {

  homePoint = digitalRead(aPoint);
  resetState = digitalRead(rButton);
  awayPoint = digitalRead(bPoint);

  if (homePoint == LOW || resetState == LOW || awayPoint == LOW) {
    currentButtonState = 1;
  }
  else {
    currentButtonState = 0;
  }


  if (currentButtonState == 1 && lastButtonState == 0) {

    if (homePoint == LOW && resetState == HIGH && awayPoint == HIGH) {
      homeScore += 1;
      lastButtonState = currentButtonState;
    }
    else if (homePoint == HIGH && resetState == HIGH && awayPoint == LOW) {
      awayScore += 1;
      lastButtonState = currentButtonState;
    }
    else if (homePoint == HIGH && resetState == LOW && awayPoint == HIGH) {
      homeScore = 0;
      awayScore = 0;
      lastButtonState = currentButtonState;
    }
    
  }

  lastButtonState = currentButtonState;

  currentScore = (homeScore * 100) + awayScore;

  if (homeScore > 99 || awayScore > 99) {
    homeScore = 0;
    awayScore = 0;
  }

  sevseg.setNumber(currentScore, 2);
  sevseg.refreshDisplay();
}