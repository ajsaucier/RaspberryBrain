// See: https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/
#include <Arduboy2.h>
#include "Images.h"
#include "global.h"
#include "game.h"
 
Arduboy2 arduboy;

void setup() {
  arduboy.boot();
  arduboy.setFrameRate(75);

  // initialize things here

}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();
  
  // game goes here
  drawBackground();
  drawTest();

  arduboy.display();
}
