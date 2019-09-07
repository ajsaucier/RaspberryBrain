#ifndef GAME_H
#define GAME_H

#include "globals.h"

// Add ability to turn on or off beep sounds
void toggleSoundSettings() {
  
  if (arduboy.audio.enabled()) {
    arduboy.audio.off();
  } else {
    arduboy.audio.on();
  }
  arduboy.audio.saveOnOff();
}

// Add ability to toggle screen flash when synapse is hit
void toggleScreenFlash() {
  
  if (saveData.shouldScreenFlash) {
    saveData.shouldScreenFlash = false;
  } else {
    saveData.shouldScreenFlash = true;
  }
  EEPROM.put(saveDataAddress, saveData);
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

// 
// Reset everything for a new game
// 
void initializeGame() {

  for (uint8_t i = 0; i < numberOfMatters; ++i) {
    matters[i].enabled = false;
  }
  
  for (uint8_t i = 0; i < numberOfSynapses; ++i) {
    synapses[i].enabled = false;
  }
  
  player.x = 0;
  player.y = (HEIGHT / 2) - (playerSize / 2);
  player.image = raspberry;
  
  score = 0;
  launchTimer = 200;
  isPaused = false;
  
  saveData.shouldScreenFlash = EEPROM.get(saveDataAddress, saveData.shouldScreenFlash);
  saveData.highScore = EEPROM.get(saveDataAddress, saveData.highScore);
}

void printInstructions() {
  arduboy.setCursor(26, 8);
  arduboy.print(F("Avoid sticky"));
  arduboy.setCursor(26, 18);
  arduboy.print(F("brain matter"));
  arduboy.setCursor(26, 28);
  arduboy.print(F("and press A when"));
  arduboy.setCursor(26, 38);
  arduboy.print(F("near synapses!"));
  arduboy.setCursor(26, 48);
  arduboy.print(F("Press B to pause."));
}

void drawPlayer() {
  Sprites::drawSelfMasked(player.x, player.y, player.image, 0);
}

void updateSynapses() {
  for (uint8_t i = 0; i < numberOfSynapses; ++i) {
    if (synapses[i].enabled) {
      synapses[i].x--;
      
      // If synapse goes off the left of screen or gets clicked on, reset it
      if (synapses[i].x < -getImageWidth(synapses[i].image) || synapses[i].hit) {
        synapses[i].enabled = false;
      }
      
      if(synapses[i].x < -getImageWidth(synapses[i].image) && !synapses[i].hit) {
        gameStatus = GameStatus::GameOver;
      }
    }
  }
}

// Draw synapse, one on screen at a time
void drawSynapses() {
  
  for (uint8_t i = 0; i < numberOfSynapses; ++i) {
    if (synapses[i].enabled) {
      Sprites::drawSelfMasked(synapses[i].x, synapses[i].y, synapses[i].image, 0);
    }
  }
}

// Move matters until they go off screen, then reset them
void updateMatters() {
  for (uint8_t i = 0; i < numberOfMatters; ++i) {
    if (matters[i].enabled) {
      --matters[i].x;
      
      // If matter goes off the left of screen, reset it
      if (matters[i].x < -getImageWidth(matters[i].image)) {
        matters[i].enabled = false;
      }
    }
  }
}

// 
// Render enabled matters on the screen
// 
void drawMatters() {

  for (uint8_t i = 0; i < numberOfMatters; ++i) {
    if (matters[i].enabled) {
      Sprites::drawSelfMasked(matters[i].x, matters[i].y, matters[i].image, 0);      
    }
  }
  
  // DEBUG
  
  /* arduboy.fillRect(0, 0, 32, 32, BLACK);
  
  for (uint8_t i = 0; i < numberOfMatters; ++i) {
    arduboy.setCursor(0, i * 10);
    arduboy.print(matters[i].enabled);
    arduboy.print(", ");
    arduboy.print(matters[i].x);
    arduboy.print(", ");
    arduboy.print(getImageWidth(matters[i].image));
  }
  
  */
  
}

// 
// Detect all collisions
// 

bool collisionMatter() { // Built-in method
        
  for (uint8_t i = 0; i < numberOfMatters; ++i) {

    if (matters[i].enabled) {

      Rect playerRect 
      {
        player.x, 
        player.y,
        getImageWidth(player.image),
        getImageHeight(player.image) 
      };
                            
      // arduboy.drawRect(playerRect.x, playerRect.y, playerRect.width, playerRect.height );
                            
                                    
      Rect matterRect
      { 
        matters[i].x, 
        matters[i].y,
        getImageWidth(matters[i].image), 
        getImageHeight(matters[i].image) 
      };
                        
      // arduboy.drawRect(obsRect.x, obsRect.y, obsRect.width, obsRect.height );

      if (arduboy.collide(playerRect, matterRect)) {
        return true;
      }
    }
  }
  return false;
}

// Synapse collision

bool collisionSynapse() {
  
  for (uint8_t i = 0; i < numberOfSynapses; ++i) {
    
    if (synapses[i].enabled) {
      
      Rect playerRect
      { 
        player.x, 
        player.y,
        getImageWidth(player.image),
        getImageHeight(player.image) 
      };
                            
      Rect synapseRect 
      { 
        synapses[i].x, 
        synapses[i].y,
        getImageWidth(synapses[i].image), 
        getImageHeight(synapses[i].image) 
      };

      if (arduboy.collide(playerRect, synapseRect)) {
        return true;
      }
    }
  }
  return false;
}

void detectHit() {
  
  for (uint8_t i = 0; i < numberOfSynapses; ++i) {
    if (collisionSynapse() && arduboy.justPressed(A_BUTTON)) {
      synapses[i].hit = true;
      
      if (saveData.shouldScreenFlash) {
        arduboy.fillScreen(WHITE);
      }
      
      if (arduboy.audio.enabled()) {
        sound.tone(NOTE_E5, 50);
      }
      score += 10;
    }
  }
}

// 
// Detect player input for raspberry movement
// 

void movePlayer() {
  
  // move left
  if (arduboy.pressed(LEFT_BUTTON) && player.x > 0 && !collisionMatter()) {
    player.x--;
  }
  
  // move right if not hitting an matter
  if (arduboy.pressed(RIGHT_BUTTON) && player.x < 100 && !collisionMatter()) {
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
  synapses[synapseNumber].x = WIDTH - 1;
  synapses[synapseNumber].y = random(8, 48);
  synapses[synapseNumber].enabled = true;
  synapses[synapseNumber].hit = false;
}

void launchMatter(uint8_t matterNumber) {
  
  // need to randomly choose between small or medium size
  uint8_t randomLowerVal = static_cast<uint8_t>(Size::Small); // 0
  uint8_t randomUpperVal = static_cast<uint8_t>(Size::Medium); // 1
  uint8_t randomObstacle = random(randomLowerVal, randomUpperVal + 1);
  Size size = static_cast<Size>(randomObstacle);
  
  // random number from spawnCoords[] lookup table
  int index = random() % (sizeof spawnCoords / sizeof *spawnCoords);
  
  // launch matter
  matters[matterNumber].x = WIDTH - 1;
  matters[matterNumber].y = spawnCoords[index];
  matters[matterNumber].size = size;
  matters[matterNumber].enabled = true;
  
  if (matters[matterNumber].size == Size::Medium) {
    matters[matterNumber].image = matterMedium;
  } else {
    matters[matterNumber].image = matterSmall;
  }

}

// 
// Functions for game states
// 

void introduction() {
  
  initializeGame();
  
  Sprites::drawOverwrite(0, 0, intro, 0);
  
  Sprites::drawOverwrite(95, 43, (arduboy.audio.enabled() ? sound_on : sound_off), 0);
  
  if (arduboy.justPressed(B_BUTTON)) {
    toggleSoundSettings();
    Sprites::drawOverwrite(95, 43, (arduboy.audio.enabled() ? sound_on : sound_off), 0);
  }
  
  Sprites::drawOverwrite(83, 53, (saveData.shouldScreenFlash ? sound_on : sound_off), 0);
  
  if (arduboy.justPressed(RIGHT_BUTTON)) {
    toggleScreenFlash();
    Sprites::drawOverwrite(83, 53, (saveData.shouldScreenFlash ? sound_on : sound_off), 0);
  }
  
  if (arduboy.justPressed(A_BUTTON)) {
    gameStatus = GameStatus::PlayGame;
  }
}

void playGame() {
  
  // include all gameplay functions in here
  drawBackground();
  drawPlayer();
  movePlayer();
  
  // Pause feature
  if (isPaused) {
    arduboy.fillScreen(BLACK);
    arduboy.setCursor(45, 20);
    arduboy.print(F("PAUSED"));
    arduboy.print(F("\n"));
    arduboy.setCursor(5, 40);
    arduboy.print(F("Press B to continue."));
    
    if (arduboy.justPressed(B_BUTTON)) {
      isPaused = false;
    }
    
    return;
  }
  
  if (arduboy.justPressed(B_BUTTON)) {
    isPaused = true;
    return;
  }

  // Don't launch anything until timer is up.
  // This is here so the player isn't caught off guard immediately
  
  if (launchTimer > 0) {
    --launchTimer;
    printInstructions();
  }
  
  if (launchTimer == 0) {
    --matterLaunchCountdown;
    
    if (matterLaunchCountdown == 0) {
      for (uint8_t i = 0; i < numberOfMatters; ++i) {
        if (!matters[i].enabled) { 
          launchMatter(i);
          break;
        }
      }
  
      matterLaunchCountdown = random(matterLaunchDelayMin, matterLaunchDelayMax);
              
    }
    
    // Should we launch another target (synapse)?
    --synapseLaunchCountdown;
    
    if (synapseLaunchCountdown == 0) {
      // Launch a new synapse if player clicked on current one
      for (uint8_t i = 0; i < numberOfSynapses; ++i) {
        if (!synapses[i].enabled) {
          launchSynapse(i);
          break;
        }
      }
      
      synapseLaunchCountdown = random(synapseLaunchDelayMin, synapseLaunchDelayMax) + matterLaunchCountdown;
      
    }
  }
  
  // Any collisions?

  if (collisionMatter()) {
      if (player.x > 0) {
        player.x--;
      } else {
        gameStatus = GameStatus::GameOver;
      }
  }
  
  updateMatters();
  
  drawMatters();
  
  updateSynapses();
  
  drawSynapses();
  
  detectHit();
  
}

void gameOver() {
  
  if (score > saveData.highScore) {
    saveData.highScore = score;
    EEPROM.put(saveDataAddress, saveData);
  }
  
  arduboy.setCursor(35, 10);
  arduboy.print(F("GAME OVER"));
  
  arduboy.setCursor(0, 30);
  arduboy.print(F("SCORE: "));
  arduboy.print(score);
  
  arduboy.setCursor(0, 40);
  arduboy.print(F("HIGH SCORE: "));
  arduboy.print(saveData.highScore);
  
  arduboy.setCursor(0, 50);
  arduboy.print(F("Press B to start over."));
  
  // Show current score and high score on this screen
  
  if (arduboy.justPressed(B_BUTTON)) {
    gameStatus = GameStatus::Introduction;
  }
}

#endif
