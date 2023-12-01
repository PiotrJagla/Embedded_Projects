#include <LiquidCrystal.h>

//2 - E
//3 - D
//4 - C
//5 - DP
//6 - B
//7 - A
//8 - F
//9 - G
int numbers[10][8] = {
  {0,0,1,0,1,0,0,0},
  {1,1,0,0,1,1,0,1},
  {0,1,1,0,1,1,0,1},
  {0,0,1,0,1,0,1,1},
  {0,1,1,0,0,1,1,1},
  {1,1,1,0,0,1,1,1},
  {0,0,1,0,1,1,0,0},
  {1,1,1,0,1,1,1,1},
  {0,1,1,0,1,1,1,1}
};

int hexPins[4] = {10,11,12,13};

int all[8] = {1,1,1,1,1,1,1,1};

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}


long tics = 0;
void loop() {
  int counter = tics / 20;
  int unitsDigit = counter % 10;
  int tensDigit = (counter % 100) / 10;
  displayNumberOnHex(tensDigit, 1);
  displayNumberOnHex(unitsDigit, 2);

  tics++;
}

void displayNumberOnHex(int number, int hexNumber){

  for(int i = 0 ; i < 4 ; ++i) {
    if(hexPins[hexNumber-1] == hexPins[i]){
      digitalWrite(hexPins[hexNumber-1], LOW);
    }
    else{
      digitalWrite(hexPins[i], HIGH);
    }
  }
  
  for(int i = 0 ; i < 8 ; i ++) {
    digitalWrite(i+2, numbers[number - 1][i]);
  }
  delay(10);
}


