#ifndef GLOBAL_H
#define GLOBAL_H

Arduboy2 arduboy;
#include "Size.h"
#include <ArduboyTones.h>
ArduboyTones sound(arduboy.audio.enabled);

constexpr uint8_t tileSize{ 32 };
constexpr uint8_t borderHeight{ 8 };
constexpr uint8_t bottomBorderLimit{ HEIGHT - borderHeight };
constexpr uint8_t playerSize{ 16 };
constexpr uint8_t numberOfMatters{ 4  };
constexpr uint16_t increaseDifficultyScore { 1000 };
constexpr uint8_t numberOfSynapses{ 1 };
constexpr uint8_t synapseSize{ 8 };

constexpr uint16_t saveOffset = 164;
constexpr uint16_t startAddress = (EEPROM_STORAGE_SPACE_START + saveOffset);

constexpr uint16_t byte1Address = startAddress;
constexpr uint16_t byte2Address = (byte1Address + sizeof(uint8_t));
constexpr uint16_t dataAddress = (byte2Address + sizeof(uint8_t));

// Some more suitable letters :P
constexpr uint8_t checkByte1 = 'R';
constexpr uint8_t checkByte2 = 'B';

// Set everything being saved to EEPROM - credit goes to Pharap and filmote
struct SaveData {
  bool shouldScreenFlash;
  uint16_t highScore;
};

SaveData saveData { false, 0 };

void clearEEPROM()
{
  EEPROM.update(byte1Address, checkByte1);
  EEPROM.update(byte2Address, checkByte2);

  SaveData blankSaveData { false, 0 };
  EEPROM.put(dataAddress, blankSaveData);
}

void initialiseEEPROM()
{
  uint8_t byte1 = EEPROM.read(byte1Address);
  uint8_t byte2 = EEPROM.read(byte2Address);

  if ((byte1 != checkByte1) || (byte2 != checkByte2))
  {
    clearEEPROM();
  }
}

void loadEEPROM()
{
  EEPROM.get(dataAddress, saveData);
}

void saveEEPROM()
{
  EEPROM.put(dataAddress, saveData);
}

// Ground scrolling
bool moveGround{ true };
float groundX { 0 };

// Pause
bool isPaused{ false };

// Scoring
uint16_t score { 0 };

// Timing of random matter spawning
uint8_t matterLaunchDelayMin{ 30 };
uint16_t matterLaunchDelayMax{ 70 };

// Timer for launching matters
uint16_t matterLaunchCountdown = matterLaunchDelayMin;

// Timing of synapse spawning
uint8_t synapseLaunchDelayMin{ 20 };
uint16_t synapseLaunchDelayMax{ 20 };

// Timer for synapse targets
uint16_t synapseLaunchCountdown = synapseLaunchDelayMin;

// Timer to delay first spawn
uint8_t launchTimer{ 200 };

// Lookup table for spawning medium matters
int spawnCoords[16] = {
  8, 9, 10, 11, 12, 13, 14, 32, 33, 34, 35, 36, 37, 38, 39, 40
};

enum class GameStatus : uint8_t {
  Reset,
  Introduction,
  PlayGame,
  GameOver,
};

GameStatus gameStatus {GameStatus::Reset}; // for production
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

// create matter matters
Matter matters[numberOfMatters] = {
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

Synapse synapses[numberOfSynapses] = {
  {100, (HEIGHT / 2) - (synapseSize / 2), false, false, synapse}
};

#endif
