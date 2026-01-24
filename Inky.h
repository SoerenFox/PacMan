#pragma once

#include "Ghost.h"

class Inky : public Ghost {

  public:
    Inky(Vector2 startPos, float radius, float speed);
    Vector2 getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition);
    
};
