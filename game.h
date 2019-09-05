#ifndef GAME_H
#define GAME_H

#include "globals.h"

bool moveGround = true;
float groundX = 0;

void pause() {
  // pause game with B button
}

// Add ability to turn on or off beep sounds
void toggleSoundSettings() {
  
  if (arduboy.audio.enabled()) {
    arduboy.audio.off();
    arduboy.audio.saveOnOff();
  } else {
    arduboy.audio.on();
    arduboy.audio.saveOnOff();
  }
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
  
  for (uint8_t i = 0; i < numberOfSynapses; i++) {
    targets[i].enabled = false;
  }
  
  player.x = 0;
  player.y = (HEIGHT / 2) - (playerSize / 2);
  player.image = raspberry;
  
  score = 0;
  launchTimer = 75;
}

void drawPlayer() {
  Sprites::drawSelfMasked(player.x, player.y, player.image, 0);
}

void updateSynapses() {
  for (uint8_t i = 0; i < numberOfSynapses; i++) {
    if (targets[i].enabled) {
      targets[i].x--;
      
      // If synapse goes off the left of screen or gets clicked on, reset it
      if (targets[i].x < -getImageWidth(targets[i].image) || targets[i].hit) {
        targets[i].enabled = false;
      }
      
      if(targets[i].x < -getImageWidth(targets[i].image) && !targets[i].hit) {
        gameStatus = GameStatus::GameOver;
      }
    }
  }
}

// Draw synapse, one on screen at a time
void drawSynapses() {
  
  for (uint8_t i = 0; i < numberOfSynapses; i++) {
    
    if (targets[i].enabled) {
      Sprites::drawSelfMasked(targets[i].x, targets[i].y, targets[i].image, 0);
    }
  }
}

// Move obstacles until they go off screen, then reset them
void updateObstacles() {
  for (uint8_t i = 0; i < numberOfObstacles; i++) {
    if (matters[i].enabled) {
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
    
    if (matters[i].enabled) {

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

    if (matters[i].enabled) {

      Rect playerRect 
      {
        player.x, 
        player.y,
        getImageWidth(player.image),
        getImageHeight(player.image) 
      };
                            
      // arduboy.drawRect(playerRect.x, playerRect.y, playerRect.width, playerRect.height );
                            
                                    
      Rect obsRect
      { 
        matters[i].x, 
        matters[i].y,
        getImageWidth(matters[i].image), 
        getImageHeight(matters[i].image) 
      };
                        
      // arduboy.drawRect(obsRect.x, obsRect.y, obsRect.width, obsRect.height );

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
    
    if (targets[i].enabled) {
      
      Rect playerRect
      { 
        player.x, 
        player.y,
        getImageWidth(player.image),
        getImageHeight(player.image) 
      };
                            
      Rect targetRect 
      { 
        targets[i].x, 
        targets[i].y,
        getImageWidth(targets[i].image), 
        getImageHeight(targets[i].image) 
      };

      if (arduboy.collide(playerRect, targetRect)) {

        return true;
              
      }
      
    }
    
  }
  
  return false;
  
}

void detectHit() {
  
  for (uint8_t i = 0; i < numberOfSynapses; i++) {
  
    if (collisionTarget() && arduboy.justPressed(A_BUTTON)) {
    
      targets[i].hit = true;
      
      arduboy.fillScreen(WHITE);
      
      if (arduboy.audio.enabled()) {
        
        sound.tone(NOTE_E5, 50);
        
      }
      
    }
  }
}

/* -----------------------------------------------
Detect player input for raspberry movement
------------------------------------------------*/

void movePlayer() {
  
  // move left
  if (arduboy.pressed(LEFT_BUTTON) && player.x > 0 && !collision()) {
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
  targets[synapseNumber].enabled = true;
  targets[synapseNumber].hit = false;
}

void launchObstacle(uint8_t obstacleNumber) {
  
  // need to randomly choose between small or medium size
  uint8_t randomLowerVal = static_cast<uint8_t>(Size::Small); // 0
  uint8_t randomUpperVal = static_cast<uint8_t>(Size::Medium); // 1
  uint8_t randomObstacle = random(randomLowerVal, randomUpperVal + 1);
  Size size = static_cast<Size>(randomObstacle);
  
  // random number from spawnCoords[] lookup table
  int index = rand() % (sizeof spawnCoords / sizeof *spawnCoords);
  
  // launch obstacle
  matters[obstacleNumber].x = WIDTH - 1;
  matters[obstacleNumber].y = spawnCoords[index];
  matters[obstacleNumber].size = size;
  matters[obstacleNumber].enabled = true;

}

/* --------------------
Game state functions
----------------------*/

void introduction() {
  
  initializeGame();
  
  Sprites::drawOverwrite(0, 0, intro, 0);
  
  Sprites::drawOverwrite(95, 43, (arduboy.audio.enabled() ? sound_on : sound_off), 0);
  
  if (arduboy.justPressed(B_BUTTON)) {
    
    toggleSoundSettings();
    Sprites::drawOverwrite(95, 43, (arduboy.audio.enabled() ? sound_on : sound_off), 0);
  }
  
  // Show intro graphic on this screen
  if (arduboy.justPressed(A_BUTTON)) {
    gameStatus = GameStatus::PlayGame;
  }
}

void playGame() {
  
  // include all gameplay functions in here
  drawBackground();
  drawPlayer();
  movePlayer();
  
  /*--------------------------------------
  Begin obstacle process
  --------------------------------------*/

  // Don't launch anything until timer is up.
  // This is here so the player isn't caught off guard immediately
  for (uint8_t i = 0; i < launchTimer; i--) {
    --launchTimer;
  }
  
  if (launchTimer == 0) {
    
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
    
    // Should we launch another target (synapse)?
    --synapseLaunchCountdown;
    
    if (synapseLaunchCountdown == 0) {
    
      // Launch a new synapse if player clicked on current one
      for (uint8_t i = 0; i < numberOfSynapses; i++) {
        
        if (!targets[i].enabled) {
          
          launchSynapse(i);
          
          break;
          
        }
        
      }
      
      synapseLaunchCountdown = random(synapseLaunchDelayMin, synapseLaunchDelayMax) + obstacleLaunchCountdown;
      
    }
  }
  
  // Any collisions?

  if (collision()) {
    
    // arduboy.setCursor(0, 0);
    // arduboy.print(F("obstacle"));
    
    // gameStatus = GameStatus::GameOver;
    
    if (player.x > 0) {
      player.x--;
    } else {
      gameStatus = GameStatus::GameOver;
    }

  }
  
  updateObstacles();
  
  drawObstacles();
  
  updateSynapses();
  
  drawSynapses();
  
  detectHit();
  
}

void gameOver() {
  arduboy.setCursor(0, 0);
  arduboy.print(F("Sorry, you got stuck"));
  arduboy.print(F("\n"));
  arduboy.print(F("on some brain matter!"));
  
  arduboy.setCursor(0, 30);
  arduboy.print(F("SCORE: "));
  
  arduboy.setCursor(0, 40);
  arduboy.print(F("HI SCORE: "));
  
  arduboy.setCursor(0, 50);
  arduboy.print(F("Press A to start over."));
  
  // Show current score and high score on this screen
  
  if (arduboy.justPressed(A_BUTTON)) {
    gameStatus = GameStatus::Introduction;
  }
}

#endif
