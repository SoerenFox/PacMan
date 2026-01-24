#include "Pacman.h"
#include "Map.h"
#include "raylib.h"
#include "Display.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Constructor: Initializes Pacman with position, radius, speed
Pacman::Pacman(Vector2 startPos, float radius, float speed) : Entity(startPos, radius, speed) {}

// Handles keyboard input to set Pacman's desired direction
void Pacman::handleInput(){
  if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) desiredDirection = Direction::RIGHT;
  if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) desiredDirection = Direction::UP;
  if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) desiredDirection = Direction::LEFT;
  if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) desiredDirection = Direction::DOWN;
}

// Updates Pacman's position, handles coin/fruit collection and sound
void Pacman::update(int map[21][21], int tileSize, Score &score){
  if (!IsSoundReady(credit)) credit = LoadSound("sounds/credit.wav");
  handleInput();

  bool isAligned = (fmod(position.x - tileSize / 2.0f, tileSize) == 0) && (fmod(position.y - tileSize / 2.0f, tileSize) == 0); // %  5/3=1 R:2 -> 5%3=2 

  int tileX = static_cast<int>(position.x / tileSize);
  int tileY = static_cast<int>(position.y / tileSize);

  if (static_cast<TileType>(map[tileY][tileX]) == TileType::Coin) {
    map[tileY][tileX] = static_cast<int>(TileType::Empty);
    score.points += 10;
    SetSoundVolume(credit, audio.getVolume());
    PlaySound(credit);
  }
  else if (static_cast<TileType>(map[tileY][tileX]) == TileType::Fruit) {
    map[tileY][tileX] = static_cast<int>(TileType::Empty);
    score.points += 100;
    SetSoundVolume(credit, audio.getVolume());
    PlaySound(credit);
  }

  int dx = 0, dy = 0;

  if (isAligned){
    switch (desiredDirection){
      case Direction::RIGHT: dx = 1; break;
      case Direction::UP:    dy = -1; break;
      case Direction::LEFT:  dx = -1; break;
      case Direction::DOWN:  dy = 1; break;
    }
    if (static_cast<TileType>(map[tileY + dy][tileX + dx]) != TileType::Wall){
      currentDirection = desiredDirection;
    }
  }

  dx = dy = 0;
  switch (currentDirection){
    case Direction::RIGHT: dx = 1; break;
    case Direction::UP:    dy = -1; break;
    case Direction::LEFT:  dx = -1; break;
    case Direction::DOWN:  dy = 1; break;
  }

  int nextTileX = tileX + dx;
  int nextTileY = tileY + dy;
                  
  if (isAligned && static_cast<TileType>(map[nextTileY][nextTileX]) == TileType::Wall){
    dx = dy = 0; // wall hit
  }

  position.x += dx * speed;
  position.y += dy * speed;
}

// Draws Pacman with mouth animation based on direction
void Pacman::draw(){
  float angleOffset = 0;
  switch (currentDirection) {
    case Direction::RIGHT: angleOffset = 0; break;
    case Direction::DOWN:  angleOffset = 90; break;
    case Direction::LEFT:  angleOffset = 180; break;
    case Direction::UP:    angleOffset = 270; break;
    default: angleOffset = 0; break;
  }
  float t = fabsf(sinf(GetTime() * 5));
  DrawCircleSector(position, radius, 55 - 35 * t + angleOffset, 305 + 35 * t + angleOffset, 60, YELLOW);
}