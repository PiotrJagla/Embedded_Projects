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

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}



void loop() {

  static int counter = 0;
  
  for(int i = 0 ; i < 8 ; ++i){
    digitalWrite(i+2, numbers[counter][i]);
  }
  delay(1000);
  counter = (counter + 1)%10;
}
