#include "game.h"
#include <raylib.h>
#include "rect.h"

const float playerSpeed = 130.0f;
const float gridSize = 50.0f;
const int windowWidth = 1200;
const int windowHeight = 800;

const int tilemapWidth = windowWidth/gridSize;
const int tilemapHeight = windowHeight/gridSize;
struct rect** tilemap;


struct rect player;


void initGame() {
  InitWindow(windowWidth,windowHeight, "basic window");
  player = (struct rect){0,0,50,50};

  tilemap = (int**)malloc(tilemapHeight * sizeof(int*));
  for(int i = 0 ; i < tilemapHeight; ++i) {
    tilemap[i] = (int*)malloc(tilemapWidth*sizeof(int));
  }
  tilemap[0][0] = 1;
  tilemap[0][2] = 1;
  tilemap[2][2] = 1;
}

void updateGame() {
    if(IsKeyDown(KEY_A)) {
        player.x -= playerSpeed * GetFrameTime();

    } else if(IsKeyDown(KEY_D)) {
        player.x += playerSpeed * GetFrameTime();
    }

    if(IsKeyDown(KEY_W)) {
        player.y -= playerSpeed * GetFrameTime();
    } else if(IsKeyDown(KEY_S)) {
        player.y += playerSpeed * GetFrameTime();
    }

}

void RenderTilemap() {
    for(int y = 0 ; y < tilemapHeight; y++) {
        for(int x = 0 ; x < tilemapWidth; x++) {
            if(tilemap[y][x] == 1) {
                DrawRectangle(x*gridSize, y*gridSize, gridSize, gridSize, WHITE);
            }
        }
    }
}

void renderGame() {
    BeginDrawing();
    ClearBackground(BLACK);

    RenderTilemap();
    DrawRect(&player, WHITE);
    DrawFPS(10,10);

    EndDrawing();
}
