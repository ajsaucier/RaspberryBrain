// See: https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/
#include <Arduboy2.h>
#include <EEPROM.h>
#include "Images.h"
#include "globals.h"
#include "game.h"

void setup() {
  
  arduboy.begin();
  // for quicker booting when developing
  // arduboy.boot();
  // arduboy.flashlight();
  
  arduboy.setFrameRate(60);

  // initialize things here
  arduboy.initRandomSeed();
  
  initialiseEEPROM();

}

void loop() {
  if (!arduboy.nextFrame())
    return;

  arduboy.clear();
  
  arduboy.pollButtons();

  switch (gameStatus) {

    case GameStatus::Introduction:
      introduction();
      break;

    case GameStatus::PlayGame:
      playGame();
      break;

    case GameStatus::GameOver:
      gameOver();
      break;
      
  }

  arduboy.display();
}
