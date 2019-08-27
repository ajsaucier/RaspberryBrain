#ifndef GAME_H
#define GAME_H

#include "globals.h"

bool moveGround = true;
int groundX = 0;

void pause() {
  // pause game with B button
}

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

void drawPlayer() {
  Sprites::drawSelfMasked(player.x, player.y, player.image, 0);
}

void movePlayer() {
  
  // move left
  if (arduboy.pressed(LEFT_BUTTON) && player.x > 0) {
    player.x = player.x - 3;
  }
  
  // move right
  if (arduboy.pressed(RIGHT_BUTTON) && player.x < 100) {
    player.x = player.x + 3;
  }
  
  // move up
  if (arduboy.pressed(UP_BUTTON) && player.y > borderHeight) {
    player.y = player.y - 3;
  }
  
  // move down
  if (arduboy.pressed(DOWN_BUTTON) && player.y < (bottomBorderLimit - playerSize)) {
    player.y = player.y + 3;
  }
}

void drawTest() {
  Sprites::drawSelfMasked(50, 42, synapse, 0);
  Sprites::drawSelfMasked(50, 8, matterSmall, 0);
  Sprites::drawSelfMasked(60, 8, matterMedium, 0);
  Sprites::drawSelfMasked(70, 8, matterLarge, 0);
}

/* --------------------
Game state functions
----------------------*/

void introduction() {
  arduboy.setCursor(0, 0);
  arduboy.print("Intro");
  // Show intro graphic on this screen, maybe instructions and sound on/off
  if (arduboy.justPressed(A_BUTTON)) {
    gameStatus = GameStatus::PlayGame;
  }
}

void playGame() {
  // include all gameplay functions in here
  drawBackground();
  drawTest();
  drawPlayer();
  movePlayer();
}

void gameOver() {
  arduboy.setCursor(0, 0);
  arduboy.print("Over");
  // Show current score and high score on this screen
}

#endif
