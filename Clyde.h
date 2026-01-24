#pragma once

#include "Ghost.h"

class Clyde : public Ghost {
  
  public:
    Clyde(Vector2 startPos, float radius, float speed);
    Vector2 getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition);
};
