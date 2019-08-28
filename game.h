#ifndef GAME_H
#define GAME_H

#include "globals.h"

bool moveGround = true;
float groundX = 0;

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
  
    groundX = groundX + 0.5;
    
    for (uint8_t x = 0; x < 5; x++) {
      for (uint8_t y = 0; y < 2; y++) {
        Sprites::drawSelfMasked(x * tileSize - groundX,  y * tileSize, brainBg, 0);
      }
    }
    
    for (uint8_t x = 0; x < 5; x++) {
      Sprites::drawSelfMasked(x * tileSize - groundX,  0, borderTop, 0);
    }
    
    for (uint8_t x = 0; x < 5; x++) {
      Sprites::drawSelfMasked(x * tileSize - groundX,  HEIGHT - borderHeight, borderBottom, 0);
    }
      
  }

}

void drawPlayer() {
  Sprites::drawSelfMasked(player.x, player.y, player.image, 0);
}

void updateObstacles() {
  for (uint8_t i = 0; i < numberOfObstacles; i++) {
    if (matters[i].enabled == true) {
      matters[i].x--;
      
      // If obstacle goes off the left of screen, reset it
      if (matters[i].x < -getImageWidth(matters[i].image)) {
        matters[i].enabled = false;
      }
    }
  }
}

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Render any visible obstacles on the screen ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void drawObstacles() {

  for (uint8_t i = 0; i < numberOfObstacles; i++) {
    
    if (matters[i].enabled == true) {

      Sprites::drawSelfMasked(matters[i].x, matters[i].y, matters[i].image, 0);      

    }
    
  }
  
}

void movePlayer() {
  
  // move left
  if (arduboy.pressed(LEFT_BUTTON) && player.x > 0) {
    player.x--;
  }
  
  // move right
  if (arduboy.pressed(RIGHT_BUTTON) && player.x < 100) {
    player.x++;
  }
  
  // move up
  if (arduboy.pressed(UP_BUTTON) && player.y > borderHeight) {
    player.y--;
  }
  
  // move down
  if (arduboy.pressed(DOWN_BUTTON) && player.y < (bottomBorderLimit - playerSize)) {
    player.y++;
  }
}

void launchObstacle(uint8_t obstacleNumber) {
  
  // launch obstacle
  matters[obstacleNumber].x = WIDTH - 1;
  matters[obstacleNumber].y = random(8, 50);
  matters[obstacleNumber].size = Size::Small;
  matters[obstacleNumber].enabled = true;
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
  // drawTest();
  drawPlayer();
  movePlayer();
  
  // Begin obstacle process
  uint16_t obstacleLaunchCountdown = obstacleLaunchDelayMin;
  
  // Should we launch another obstacle?
  
  // Isn't counting down for some reason...
  --obstacleLaunchCountdown;
  
  arduboy.fillRect(0, 0, 32, 8, BLACK);
  arduboy.setCursor(0, 0);
  arduboy.print(obstacleLaunchCountdown);
  
  if (obstacleLaunchCountdown == 0) {

    for (uint8_t i = 0; i < numberOfObstacles; i++) {

      if (!matters[i].enabled) { 
        launchObstacle(i); 
        break;
      }

    }

    obstacleLaunchCountdown = random(obstacleLaunchDelayMin, obstacleLaunchDelayMax);
            
  }
  
  updateObstacles();
  drawObstacles();
  
}

void gameOver() {
  arduboy.setCursor(0, 0);
  arduboy.print("Over");
  // Show current score and high score on this screen
}

#endif
