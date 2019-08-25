// See: https://mlxxxp.github.io/documents/Arduino/libraries/Arduboy2/Doxygen/html/
#include <Arduboy2.h>
#include "Images.h"

constexpr int tileSize{ 32 };
bool moveGround = true;
int groundX = 0;
 
Arduboy2 arduboy;

void drawBackground() {
  
  // Check if ground should be moved
  if (moveGround) {
    
    // If the ground has moved one entire tile, add new tiles to the end
    if (groundX == 32) {
      groundX = 0;
      
    }
  
    groundX++;
    
    
  }
  
  arduboy.fillRect(0, 0, 48, 8, BLACK);
	arduboy.setCursor(0, 0);
	
	arduboy.print(groundX);
  
  // Draw background for full screen
  for (int x = 0; x <= WIDTH; x++) {
    for (int y = 0; y <= HEIGHT; y++) {
      Sprites::drawSelfMasked(x * tileSize - groundX, y * tileSize, bg, 0);
    }
  }

}

void setup() {
  arduboy.boot();
  arduboy.setFrameRate(30);

  // initialize things here

}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();

  drawBackground();

  arduboy.display();
}
