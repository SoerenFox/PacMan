#pragma once

#include "Ghost.h"

class Blinky : public Ghost {
  
  public:
    Blinky(Vector2 startPos, float radius, float speed);
    Vector2 getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition);
    Vector2 getPosition();
};
