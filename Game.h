#pragma once

#include "Map.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "Leaderboard.h"
#include "Display.h"
#include "GameState.h"
#include "Audio.h"

#include <vector>

#define LEADERBOARD_PATH "leaderboard.txt"

class Game {
  
  Map map;
  Pacman pacman;
  std::vector<Ghost*> ghosts;
  Leaderboard leaderboard;
  Display display;
  GameState gameState;
  Audio audio;
  float volume; 

  void checkEnd(Score &currentScore);
  void gameLoop(Score &currentScore);
  void reset();

  public:
    Game();
    void run();
}; 