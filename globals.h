#ifndef GLOBAL_H
#define GLOBAL_H

Arduboy2 arduboy;

constexpr uint8_t tileSize{ 32 };
constexpr uint8_t borderHeight{ 8 };
constexpr uint8_t bottomBorderLimit{ HEIGHT - borderHeight };
constexpr uint8_t playerSize{ 16 };
constexpr uint8_t numberOfObstacles{ 4 };
constexpr uint16_t increaseDifficultyScore { 1000 };
constexpr uint8_t numberOfSynapses{ 1 };

// Scoring
uint16_t score { 0 };
uint16_t highScore { 0 };

// Timing of random obstacle spawning
uint8_t obstacleLaunchDelayMin{ 40 };
uint16_t obstacleLaunchDelayMax{ 100 };

// Timer for launching obstacles
uint16_t obstacleLaunchCountdown = obstacleLaunchDelayMin;

// Timing of synapse spawning
uint8_t synapseLaunchDelayMin{ 120 };
uint16_t synapseLaunchDelayMax{ 120 };

// Timer for synapse targets
uint16_t synapseLaunchCountdown = synapseLaunchDelayMin;

enum class GameStatus : uint8_t {
  Introduction,
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
Raspberry player = {
  0,
  (HEIGHT / 2) - (playerSize / 2),
  raspberry
};

struct Matter {
  int8_t x;
  uint8_t y;
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
  uint8_t y;
  bool hit;
  const uint8_t *image;
};

Synapse targets[numberOfSynapses] = {
  {100, 25, false, synapse}
};

#endif
