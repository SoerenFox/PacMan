#include "Pinky.h"

// Constructor: Initializes Pinky with position, radius, and speed
Pinky::Pinky(Vector2 startPos, float radius, float speed) : Ghost(startPos, radius, speed, PINK) {}

// Returns the target position for Pinky (ahead of Pacman)
Vector2 Pinky::getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition) {
  switch (direction) {
    case Direction::UP:
      return {pacmanPosition.x, pacmanPosition.y - 48 * 2.0f};
    case Direction::DOWN:
      return {pacmanPosition.x, pacmanPosition.y + 48 * 2.0f};
    case Direction::LEFT:
      return {pacmanPosition.x - 48 * 2.0f, pacmanPosition.y};
    case Direction::RIGHT:
      return {pacmanPosition.x + 48 * 2.0f, pacmanPosition.y};
    default:
      return pacmanPosition;
  }
}