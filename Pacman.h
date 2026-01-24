#pragma once

#include "Entity.h"
#include "Leaderboard.h"
#include "Audio.h"


class Pacman : public Entity {
  
  Audio audio;
  Sound credit = { 0 };
  
  void handleInput();

  public:
    Pacman(Vector2 startPos, float radius, float speed);
    void update(int map[21][21], int tileSize, Score &score);
    void draw();
};