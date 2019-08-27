#ifndef GAME_H
#define GAME_H

#include "global.h"

bool moveGround = true;
int groundX = 0;

void drawBackground() {
  
  // Check if ground should be moved
  if (moveGround) {
    
    // If the ground has moved one entire tile, add new tiles to the end
    if (groundX == 32) {
      groundX = 0;
      
    }
  
    groundX++;
    
    for (uint8_t x = 0; x <= WIDTH; x++) {
      for (uint8_t y = 0; y <= HEIGHT; y++) {
        Sprites::drawSelfMasked(x * tileSize - groundX,  y * tileSize, brainBg, 0);
      }
    }
    
    for (uint8_t x = 0; x <= WIDTH; x++) {
      Sprites::drawSelfMasked(x * tileSize - groundX,  0, borderTop, 0);
    }
    
    for (uint8_t x = 0; x <= WIDTH; x++) {
      Sprites::drawSelfMasked(x * tileSize - groundX,  HEIGHT - borderHeight, borderBottom, 0);
    }
      
  }

}

void drawTest() {
  Sprites::drawOverwrite(30, 40, raspberry, 0);
  Sprites::drawSelfMasked(50, 42, synapse, 0);
  Sprites::drawSelfMasked(50, 8, matterSmall, 0);
  Sprites::drawSelfMasked(60, 8, matterMedium, 0);
  Sprites::drawSelfMasked(70, 8, matterLarge, 0);
}

#endif
