#include "Map.h"
#include "raylib.h"

#include <cstdlib>
#include <ctime>

// Constructor: Initializes the map layout and screen dimensions
Map::Map() : screenWidth(tileCount * tileSize), screenHeight(tileCount * tileSize),
  map{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1,0,1},
    {1,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1},
    {1,0,1,1,0,0,0,1,5,5,5,5,5,1,0,0,0,1,1,0,1},
    {1,0,1,1,0,1,1,1,5,1,1,1,5,1,1,1,0,1,1,0,1},
    {1,0,0,0,0,0,0,0,5,1,1,1,5,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,0,1,5,1,1,1,5,1,0,1,0,1,1,0,1},
    {1,0,1,0,0,1,0,1,5,5,5,5,5,1,0,1,0,0,1,0,1},
    {1,0,1,0,1,1,0,1,1,1,5,1,1,1,0,1,1,0,1,0,1},
    {1,0,1,0,1,0,0,0,0,0,5,0,0,0,0,0,1,0,1,0,1},
    {1,0,0,0,0,0,1,1,0,1,0,1,0,1,1,0,0,0,0,0,1},
    {1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1},
    {1,0,1,0,1,1,0,0,0,1,0,1,0,0,0,1,1,0,1,0,1},
    {1,0,1,0,1,1,0,1,1,1,0,1,1,1,0,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  } {}

// Draws the map, including walls, coins, and fruit
void Map::draw(){
  for (int y = 0; y < tileCount; y++) {
    for (int x = 0; x < tileCount; x++) {
      if (static_cast<TileType>(map[y][x]) == TileType::Wall){
        DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, DARKGRAY);
      }
      else if (static_cast<TileType>(map[y][x]) == TileType::Coin){
        DrawCircle(x * tileSize + tileSize / 2, y * tileSize + tileSize / 2, 4, PINK);
      }
      else if (static_cast<TileType>(map[y][x]) == TileType::Fruit){
        drawFruit({static_cast<float>(x * tileSize + tileSize / 2), static_cast<float>(y * tileSize + tileSize / 2)});
      }
    }
  }
}

// Randomly spawns a fruit on the map
void Map::spawnFruit(){
  if (std::rand() % 400 == 0){
    int xRand = std::rand() % 16;
    int yRand = std::rand() % 16;

    if (static_cast<TileType>(map[yRand][xRand]) == TileType::Empty) map[yRand][xRand] = static_cast<int>(TileType::Fruit);
  }
}

// Resets the map to its initial state
void Map::reset(){
  for (int y = 0; y < tileCount; y++){
    for (int x = 0; x < tileCount; x++){
      if (static_cast<TileType>(map[y][x]) == TileType::Fruit || static_cast<TileType>(map[y][x]) == TileType::Empty) map[y][x] = static_cast<int>(TileType::Coin);
    }
  }
} 

// Draws a fruit (cherry) at the given position
void Map::drawFruit(Vector2 pos){
  int radius = 10;
  radius /= 2;
  // Draw two red circles for cherries
  DrawCircleV({pos.x - radius * 0.8f, pos.y}, radius, RED);
  DrawCircleV({pos.x + radius * 0.8f, pos.y}, radius, RED);
  // Draw stems
  DrawLineBezier({pos.x - radius * 0.8f, pos.y - radius / 4 * 1.0f}, {pos.x, pos.y - radius * 2.5f}, radius * 0.3f, DARKGREEN);
  DrawLineBezier({pos.x + radius * 0.8f, pos.y - radius * 1.0f}, {pos.x, pos.y - radius * 2.5f}, radius * 0.3f, DARKGREEN);
  // Draw a small highlight
  DrawCircleV({pos.x - radius * 1.2f, pos.y - radius * 0.4f}, radius * 0.3f, WHITE);
  DrawCircleV({pos.x + radius * 0.4f, pos.y - radius * 0.4f}, radius * 0.3f, WHITE);
}