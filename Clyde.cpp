#include "Clyde.h"

// Constructor: Initializes Clyde with position, radius, and speed
Clyde::Clyde(Vector2 startPos, float radius, float speed) : Ghost(startPos, radius, speed, ORANGE) {}

// Returns the target position for Clyde (chases Pacman if far, else goes to corner)
Vector2 Clyde::getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition) {
  float dx = position.x - pacmanPosition.x;
  float dy = position.y - pacmanPosition.y;
  float distance = sqrt(dx * dx + dy * dy) / 48;

  if (distance > 6.0f) {
    return pacmanPosition;
  } else {
    return {1, 1}; 
  }
}
