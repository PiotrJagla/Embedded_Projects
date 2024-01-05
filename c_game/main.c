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
