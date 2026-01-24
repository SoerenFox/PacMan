#include "Ghost.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <queue>
#include <algorithm>
#include <iostream>

// Constructor: Initializes Ghost with position, radius, speed, color, and sets initial direction
Ghost::Ghost(Vector2 startPos, float radius, float speed, Color color) : Entity(startPos, radius, speed), bodyColor(color) {
  currentDirection = Direction::UP;
}

// Updates the ghost's movement and sound state
void Ghost::update(Vector2 destination, int map[21][21], int tileSize) {
  if (!IsSoundReady(chasing)) chasing = LoadSound("sounds/siren_1.wav");
  if (!IsSoundReady(scattering)) scattering = LoadSound("sounds/regsiren.mp3");
  bool isAligned = (fmod(position.x - tileSize / 2.0f, tileSize) == 0) && (fmod(position.y - tileSize / 2.0f, tileSize) == 0);

  int tileX = static_cast<int>(position.x / tileSize);
  int tileY = static_cast<int>(position.y / tileSize);

  bool prevChaseMode = chaseMode; // Store previous state

  if (isAligned){
    if (((time(NULL) / 20) % 2) == 0) {
      currentDirection = getRandomDirection(map);
      chaseMode = false; // Random movement
    } else {
      currentDirection = getDirection(destination, map, tileSize);
      chaseMode = true; // Chase mode
    }
  }

  if (chaseMode != prevChaseMode) {
    if (chaseMode) {
      // Play sound for switching to chase mode
      SetSoundVolume(chasing, audio.getVolume());
      PlaySound(chasing);
    } else {
      // Play sound for switching to scatter mode
      SetSoundVolume(scattering, audio.getVolume());
      PlaySound(scattering);
    }
  }


  int dx = 0, dy = 0;
  switch (currentDirection){
    case Direction::RIGHT: dx = 1; break;
    case Direction::UP:    dy = -1; break;
    case Direction::LEFT:  dx = -1; break;
    case Direction::DOWN:  dy = 1; break;
  }

  int nextTileX = tileX + dx;
  int nextTileY = tileY + dy;

  if (isAligned && static_cast<TileType>(map[nextTileY][nextTileX]) == TileType::Wall) {
    dx = dy = 0; // Hit wall, stop moving
  }

  position.x += dx * speed;
  position.y += dy * speed;
}

// Picks a random direction for the ghost (avoiding walls and reverse)
Direction Ghost::getRandomDirection(int map[21][21]){
  std::vector<Direction> possibleDirections;
  int tileX = static_cast<int>(position.x / 48);
  int tileY = static_cast<int>(position.y / 48);

  if (map[tileY][tileX + 1] != static_cast<int>(TileType::Wall) && currentDirection != Direction::LEFT) possibleDirections.push_back(Direction::RIGHT);
  if (map[tileY][tileX - 1] != static_cast<int>(TileType::Wall) && currentDirection != Direction::RIGHT) possibleDirections.push_back(Direction::LEFT);
  if (map[tileY - 1][tileX] != static_cast<int>(TileType::Wall) && currentDirection != Direction::DOWN) possibleDirections.push_back(Direction::UP);
  if (map[tileY + 1][tileX] != static_cast<int>(TileType::Wall) && currentDirection != Direction::UP) possibleDirections.push_back(Direction::DOWN);

  int index = std::rand() % possibleDirections.size();
  return possibleDirections[index];
}

// Draws the ghost with its body, eyes, and pupils
void Ghost::draw(){
  constexpr Vector2 directionTable[] = {
    {0, -1},  // UP
    {0, 1},   // DOWN
    {-1, 0},  // LEFT
    {1, 0}    // RIGHT
  };
  Vector2 dir = directionTable[static_cast<int>(currentDirection)];

  DrawCircle(position.x, position.y, radius, bodyColor);
  DrawRectangle(position.x - radius, position.y, radius * 2, radius, BLACK);
  DrawRectangle(position.x - radius, position.y, radius * 2 , radius - radius / 4, bodyColor);
  for (int i = 0; i < 4; i++) DrawCircle(position.x - radius + radius / 4 + radius / 2 * i, position.y + radius - radius / 4 + (int)(radius / 20 * sinf(GetTime() * 2 + i * 1.5)), radius / 4, bodyColor);

  DrawCircle(position.x - radius / 2.5, position.y - radius / 2.5, radius / 3.5, WHITE);
  DrawCircle(position.x + radius / 2.5, position.y - radius / 2.5, radius / 3.5, WHITE);

  DrawCircle(position.x - radius / 2.5 + dir.x * radius / 8, position.y - radius / 2.5 + dir.y * radius / 8, radius / 7, BLUE);
  DrawCircle(position.x + radius / 2.5 + dir.x * radius / 8, position.y - radius / 2.5 + dir.y * radius / 8, radius / 7, BLUE);
}

// Uses A* to find the direction toward the destination
Direction Ghost::getDirection(Vector2 destination, int map[21][21], int tileSize) {
  int startX = static_cast<int>(position.x / tileSize);
  int startY = static_cast<int>(position.y / tileSize);
  int destX = static_cast<int>(destination.x / tileSize);
  int destY = static_cast<int>(destination.y / tileSize);

  // Check if start or destination is out of bounds or a wall
  if (startX < 0 || startX >= 21 || startY < 0 || startY >= 21 ||
      destX < 0 || destX >= 21 || destY < 0 || destY >= 21 ||
      static_cast<TileType>(map[destY][destX]) == TileType::Wall) {
    return getRandomDirection(map);
  }

  // Lambda for heuristic (Manhattan distance)
  auto heuristic = [](int x1, int y1, int x2, int y2) { return abs(x1 - x2) + abs(y1 - y2); }; // functino defintion that calculates the heuristic value between two points

  struct Node {
    int x, y, g, h;
    Node* parent;
    int f() const { return g + h; }
  };

  auto cmp = [](Node* a, Node* b) { return a->f() > b->f(); }; // function defintion that returns true if a has a higher f value than b
  std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> open(cmp);
  bool closed[21][21] = {false};
  Node* nodes[21][21] = {nullptr};

  Node* start = new Node{startX, startY, 0, heuristic(startX, startY, destX, destY), nullptr};
  open.push(start);
  nodes[startY][startX] = start;

  int dxs[4] = {1, -1, 0, 0};
  int dys[4] = {0, 0, 1, -1};

  std::vector<std::pair<int, int>> path;

  while (!open.empty()) {
    Node* curr = open.top(); open.pop();
    // Extra bounds check for curr
    if (curr->x < 0 || curr->x >= 21 || curr->y < 0 || curr->y >= 21) {
      continue;
    }
    if (curr->x == destX && curr->y == destY) {
      // Reconstruct path
      while (curr) {
        path.push_back({curr->x, curr->y});
        curr = curr->parent;
      }
      std::reverse(path.begin(), path.end());
      break;
    }

    closed[curr->y][curr->x] = true;
    for (int dir = 0; dir < 4; ++dir) {
      int nx = curr->x + dxs[dir], ny = curr->y + dys[dir];
      // Extra bounds check for neighbor
      if (nx < 0 || nx >= 21 || ny < 0 || ny >= 21) continue; // outside of map
      if (closed[ny][nx]) continue; // already evaluated
      if (static_cast<TileType>(map[ny][nx]) == TileType::Wall) continue; // wall
      int g = curr->g + 1;
      int h = heuristic(nx, ny, destX, destY);
      if (!nodes[ny][nx] || g < nodes[ny][nx]->g) {
        if (nodes[ny][nx]) delete nodes[ny][nx];
        nodes[ny][nx] = new Node{nx, ny, g, h, curr};
        open.push(nodes[ny][nx]);
      }
    }
  }

  if (path.size() > 1){
    int nextTileX = path[1].first;
    int nextTileY = path[1].second;

      for (int y = 0; y < 21; ++y)
        for (int x = 0; x < 21; ++x)
          if (nodes[y][x]) delete nodes[y][x];

    if ((nextTileX - startX) == -1) return Direction::LEFT;
    if ((nextTileX - startX) == 1) return Direction::RIGHT;
    if ((nextTileY - startY) == -1) return Direction::UP;
    if ((nextTileY - startY) == 1) return Direction::DOWN;
  }
  
  return currentDirection; 
}

// Returns the default target (Pacman position)
Vector2 Ghost::getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition) {
  return pacmanPosition;
}

// Returns the ghost's current position
Vector2 Ghost::getPosition() {
  return position;
}