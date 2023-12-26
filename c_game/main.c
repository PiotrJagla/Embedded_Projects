#include <raylib.h>
#include <stdio.h>
#include <pthread.h>
#include "game.h"


int main() {

  initGame();

  while(!WindowShouldClose()){
    updateGame();
    renderGame();
  }

  CloseWindow();

  return 0;
}
