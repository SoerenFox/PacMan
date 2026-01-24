#pragma once

#include <vector>
#include <ctime>

#include "Entity.h"
#include "Map.h"
#include "Audio.h"


class Ghost : public Entity {

  protected:
    bool chaseMode;
    Audio audio;
    Sound chasing = { 0 };
    Sound scattering = { 0 };
    Color bodyColor;

    Direction getRandomDirection(int map[21][21]);
    Direction getDirection(Vector2 destination, int map[21][21], int tileSize);

  public:
    Ghost(Vector2 startPos, float radius, float speed, Color color);
    void update(Vector2 destination, int map[21][21], int tileSize);
    virtual void draw();
    virtual Vector2 getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition);
    Vector2 getPosition();
};

