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
  
    groundX++;
    
    // Background design might be too overwhelming
    // for (uint8_t x = 0; x < 5; x++) {
    //   for (uint8_t y = 0; y < 2; y++) {
    //     Sprites::drawSelfMasked((x * tileSize) - groundX,  y * tileSize, brainBg, 0);
    //   }
    // }
    
    for (uint8_t x = 0; x < 5; x++) {
      Sprites::drawSelfMasked((x * tileSize) - groundX,  0, borderTop, 0);
    }
    
    for (uint8_t x = 0; x < 5; x++) {
      Sprites::drawSelfMasked((x * tileSize) - groundX,  HEIGHT - borderHeight, borderBottom, 0);
    }
      
  }

}

/* -----------------------------------------------------------------------------------------------------------------------------
 *  Reset everything ready for a new game ..
 * -----------------------------------------------------------------------------------------------------------------------------
 */
void initializeGame() {

  for (uint8_t i = 0; i < numberOfObstacles; i++) {
    matters[i].enabled = false;
  }
  
  score = 0;
}

void drawPlayer() {
  Sprites::drawSelfMasked(player.x, player.y, player.image, 0);
}

void updateSynapses() {
  for (uint8_t i = 0; i < numberOfSynapses; i++) {
    if (targets[i].hit == false) {
      targets[i].x--;
      
      // If synapse goes off the left of screen or gets clicked on, reset it
      if (targets[i].x < -getImageWidth(targets[i].image) || targets[i].hit == true) {
        targets[i].hit = false;
      }
    }
  }
}

// Draw synapse, one on screen at a time
void drawSynapses() {
  for (uint8_t i = 0; i < numberOfSynapses; i++) {
    Sprites::drawSelfMasked(targets[i].x, targets[i].y, targets[i].image, 0);
  }
}

// Move obstacles until they go off screen, then reset them
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

      if (matters[i].size == Size::Medium) {
        matters[i].image = matterMedium;
      } else {
        matters[i].image = matterSmall;
      }
      
      Sprites::drawSelfMasked(matters[i].x, matters[i].y, matters[i].image, 0);      

    }
    
  }
  
  // DEBUG
  
  /* arduboy.fillRect(0, 0, 32, 32, BLACK);
  
  for (uint8_t i = 0; i < numberOfObstacles; i++) {
    arduboy.setCursor(0, i * 10);
    arduboy.print(matters[i].enabled);
    arduboy.print(", ");
    arduboy.print(matters[i].x);
    arduboy.print(", ");
    arduboy.print(getImageWidth(matters[i].image));
  }
  
  */
  
}

/* -----------------------------------------------
Detect collisions
------------------------------------------------*/

bool collision() { // Built-in method
        
  for (uint8_t i = 0; i < numberOfObstacles; i++) {

    if (matters[i].enabled == true) {

      Rect playerRect = Rect{ player.x, 
                            player.y - getImageHeight(player.image),
                            getImageWidth(player.image),
                            getImageHeight(player.image) };
                            
                                    
      Rect obsRect =  Rect{ matters[i].x, 
                        matters[i].y - getImageHeight(matters[i].image),
                        getImageWidth(matters[i].image), 
                        getImageHeight(matters[i].image) };
                            
      if (matters[i].size == Size::Medium) {
        
        arduboy.setCursor(50, 0);
        arduboy.print(F("medium"));
        arduboy.print("\n");
        arduboy.print(getImageWidth(matters[i].image));
        arduboy.print(F(", "));
        arduboy.print(getImageHeight(matters[i].image));
        
      }

      if (arduboy.collide(playerRect, obsRect)) {

        return true;
              
      }
          
    }
        
  }

  return false;

}

// Synapse collision

bool collisionTarget() {
  
    for (uint8_t i = 0; i < numberOfSynapses; i++) {
    
    if (!targets[i].hit) {
      
      Rect playerRect = Rect{ player.x, 
                            player.y - getImageHeight(player.image),
                            getImageWidth(player.image),
                            getImageHeight(player.image) };
      Rect targetRect = Rect{ targets[i].x, 
                            targets[i].y - getImageHeight(targets[i].image),
                            getImageWidth(targets[i].image), 
                            getImageHeight(targets[i].image) };

      if (arduboy.collide(playerRect, targetRect)) {

        return true;
              
      }
      
    }
    
  }
  
  return false;
  
}

/* -----------------------------------------------
Detect player input for raspberry movement
------------------------------------------------*/

void movePlayer() {
  
  // move left
  if (arduboy.pressed(LEFT_BUTTON) && player.x > 0) {
    player.x--;
  }
  
  // move right if not hitting an obstacle
  if (arduboy.pressed(RIGHT_BUTTON) && player.x < 100 && !collision()) {
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

void launchSynapse(uint8_t synapseNumber) {
  // launch synapse target
  targets[synapseNumber].x = WIDTH - 1;
  targets[synapseNumber].y = random(8, 48);
  targets[synapseNumber].hit = false;
}

void launchObstacle(uint8_t obstacleNumber) {
  
  // need to randomly choose between small or medium size
  uint8_t randomLowerVal = static_cast<uint8_t>(Size::Small); // 0
  uint8_t randomUpperVal = static_cast<uint8_t>(Size::Medium); // 1
  uint8_t randomObstacle = random(randomLowerVal, randomUpperVal + 1);
  Size size = static_cast<Size>(randomObstacle);
  
  // launch obstacle
  matters[obstacleNumber].x = WIDTH - 1;
  matters[obstacleNumber].y = random(0, 50);
  matters[obstacleNumber].size = size;
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
  
  initializeGame();
  
  arduboy.setCursor(0, 0);
  arduboy.print(F("Intro"));
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
  
      // Should we launch another obstacle?
  --obstacleLaunchCountdown;
  
  if (obstacleLaunchCountdown == 0) {

    for (uint8_t i = 0; i < numberOfObstacles; i++) {

      if (!matters[i].enabled) { 
        launchObstacle(i); 
        break;
      }

    }

    obstacleLaunchCountdown = random(obstacleLaunchDelayMin, obstacleLaunchDelayMax);
            
  }
  
      // Should we launch another obstacle?
  --synapseLaunchCountdown;
  
  if (synapseLaunchCountdown == 0) {
  
    // Launch a new synapse if player clicked on current one
    for (uint8_t i = 0; i < numberOfSynapses; i++) {
      if (!targets[i].hit) {
        launchSynapse(i);
        break;
      }
      
    }
    
    synapseLaunchCountdown = random(synapseLaunchDelayMin, synapseLaunchDelayMax);
    
  }
  
  // Any collisions?

  if (collision()) {
    
    arduboy.setCursor(0, 0);
    arduboy.print(F("obstacle"));
    
    // gameStatus = GameStatus::GameOver;
    
    if (player.x > 0) {
      player.x--;
    }

  }
  
  if (collisionTarget()) {
    
    arduboy.setCursor(0, 16);
    arduboy.print(F("synapse"));
  }
  
  updateObstacles();
  
  drawObstacles();
  
  updateSynapses();
  
  drawSynapses();
  
}

void gameOver() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Over"));
  // Show current score and high score on this screen
}

#endif
