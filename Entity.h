#pragma once

#include "raylib.h"

enum class Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

class Entity {

  protected:
    float speed;
    float radius;
    Vector2 position;
    Direction currentDirection;
    Direction desiredDirection;
  
  public:
    Entity(Vector2 startPos, float radius, float speed);
    virtual void draw(Color colour = YELLOW);
    virtual void update();
    Vector2 getPosition();
    Direction getCurrentDirection() { return currentDirection; }
    void setCurrentDirection(Direction dir) { currentDirection = dir; }
    float getX();
    float getY();
};