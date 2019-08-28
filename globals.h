#ifndef GLOBAL_H
#define GLOBAL_H

Arduboy2 arduboy;

constexpr uint8_t tileSize{ 32 };
constexpr uint8_t borderHeight{ 8 };
constexpr uint8_t bottomBorderLimit{ HEIGHT - borderHeight };
constexpr uint8_t playerSize{ 16 };
constexpr uint8_t numberOfObstacles{ 4 };
constexpr uint8_t obstacleLaunchDelayMin{ 90 };
constexpr uint16_t obstacleLaunchDelayMax{ 200 };

enum class GameStatus : uint8_t {
  Introduction,
  PlayGame,
  GameOver,
};

// GameStatus gameStatus {GameStatus::Introduction}; // for production
GameStatus gameStatus {GameStatus::PlayGame}; // for testing

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
  uint8_t x;
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
  uint8_t x;
  uint8_t y;
  bool hit;
  const uint8_t *image;
};

#endif
