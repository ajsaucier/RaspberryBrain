#ifndef GLOBAL_H
#define GLOBAL_H

constexpr uint8_t tileSize{ 32 };
constexpr uint8_t borderHeight{ 8 };

enum class GameStatus : uint8_t {
  Introduction,
  PlayGame,
  GameOver,
};

#endif
