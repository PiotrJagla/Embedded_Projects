#include "game.h"
#include <raylib.h>
#include "rect.h"
#include <stdlib.h>
#include "colors.h"

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
  player = (struct rect){6*gridSize,6*gridSize,50,50,GREEN};

  tilemap = (struct rect**)malloc(tilemapHeight * sizeof(struct rect*));
  for(int i = 0 ; i < tilemapHeight; ++i) {
    tilemap[i] = (struct rect*)malloc(tilemapWidth*sizeof(struct rect));
  }

  PrintColor(WHITE);

  for(int y = 0 ; y < tilemapHeight ; ++y) {
    for(int x = 0 ; x < tilemapHeight ; ++x) {
      tilemap[y][x] = (struct rect){x*gridSize, y*gridSize, gridSize, gridSize, BLACK};
    }

  }
  tilemap[0][0].c = WHITE;
  tilemap[0][2].c = WHITE;
  tilemap[2][2].c = WHITE;
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

    //check tilemap collision
    for(int y = 0 ; y < tilemapHeight ; ++y) {
      for(int x = 0 ; x < tilemapWidth; ++x) {
        struct rect tile = tilemap[y][x];
        if(CompareColors(tile.c, WHITE)) {
          //left wall
          if(tile.x < player.x + player.width) {
            player.x = tile.x - player.width;

          }

        }

      }
    }

}


void RenderTilemap() {
    for(int y = 0 ; y < tilemapHeight; y++) {
        for(int x = 0 ; x < tilemapWidth; x++) {
          struct rect tile = tilemap[y][x];
            if(CompareColors(tile.c, WHITE)) {
                DrawRectangle(tile.x, tile.y, tile.width, tile.height, tile.c);
            }
            DrawRect(&tile);
        }
    }
}

void renderGame() {
    BeginDrawing();
    ClearBackground(BLACK);

    RenderTilemap();
    DrawRect(&player);
    DrawFPS(10,10);

    EndDrawing();
}
