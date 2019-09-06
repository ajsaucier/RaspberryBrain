#ifndef GLOBAL_H
#define GLOBAL_H

Arduboy2 arduboy;
#include <ArduboyTones.h>
ArduboyTones sound(arduboy.audio.enabled);

constexpr uint8_t tileSize{ 32 };
constexpr uint8_t borderHeight{ 8 };
constexpr uint8_t bottomBorderLimit{ HEIGHT - borderHeight };
constexpr uint8_t playerSize{ 16 };
constexpr uint8_t numberOfObstacles{ 4 };
constexpr uint16_t increaseDifficultyScore { 1000 };
constexpr uint8_t numberOfSynapses{ 1 };
constexpr uint8_t synapseSize{ 8 };

// Scoring
uint16_t score { 0 };
uint16_t highScore { 0 };

// Timing of random obstacle spawning
uint8_t obstacleLaunchDelayMin{ 30 };
uint16_t obstacleLaunchDelayMax{ 70 };

// Timer for launching obstacles
uint16_t obstacleLaunchCountdown = obstacleLaunchDelayMin;

// Timing of synapse spawning
uint8_t synapseLaunchDelayMin{ 20 };
uint16_t synapseLaunchDelayMax{ 20 };

// Timer for synapse targets
uint16_t synapseLaunchCountdown = synapseLaunchDelayMin;

// Timer to delay first spawn
uint8_t launchTimer{ 100 };

// Turning screen flashing on or off
bool shouldScreenFlash{ true };

// Lookup table for spawning medium obstacles
int spawnCoords[16] = {
  8, 9, 10, 11, 12, 13, 14, 32, 33, 34, 35, 36, 37, 38, 39, 40
};

enum class GameStatus : uint8_t {
  Introduction,
  Instructions,
  PlayGame,
  GameOver,
};

GameStatus gameStatus {GameStatus::Introduction}; // for production
// GameStatus gameStatus {GameStatus::PlayGame}; // for testing

enum class Size : uint8_t {
  Small,
  Medium,
};

struct Raspberry {
  uint8_t x;
  uint8_t y;
  const uint8_t *image;
};

// create player
Raspberry player = {0, (HEIGHT / 2) - (playerSize / 2), raspberry};

struct Matter {
  int8_t x;
  int8_t y;
  Size size;
  bool enabled;
  const uint8_t *image;
};

// create matter obstacles
Matter matters[numberOfObstacles] = {
  {0, 8, Size::Small, false, matterSmall},
  {0, 8, Size::Small, false, matterSmall},
  {0, 8, Size::Small, false, matterSmall},
  {0, 8, Size::Small, false, matterSmall},
};

struct Synapse {
  int8_t x;
  int8_t y;
  bool enabled;
  bool hit;
  const uint8_t *image;
};

Synapse targets[numberOfSynapses] = {
  {100, (HEIGHT / 2) - (synapseSize / 2), false, false, synapse}
};

#endif
