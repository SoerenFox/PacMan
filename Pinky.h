#pragma once

#include "Ghost.h"

class Pinky : public Ghost {

  public:
    Pinky(Vector2 startPos, float radius, float speed);
    Vector2 getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition);
};
