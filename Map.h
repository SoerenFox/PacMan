#pragma once

#include <vector>
#include "raylib.h"

enum class TileType {
  Coin,
  Wall,
  Empty,
  Fruit,
  ForcedEmpty
};

class Map {
  void drawFruit(Vector2 pos);

  public:
    static constexpr int tileCount = 21;
    static constexpr int tileSize = 48;
    const int screenWidth;
    const int screenHeight;
    int map[tileCount][tileCount];

    Map();
    void draw();
    void spawnFruit();
    void reset();
};