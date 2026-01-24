#include "Blinky.h"

// Constructor: Initializes Blinky with position, radius, and speed
Blinky::Blinky(Vector2 startPos, float radius, float speed) : Ghost(startPos, radius, speed, RED) {}

// Returns the target position for Blinky (chases Pacman directly)
Vector2 Blinky::getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition) {
  return pacmanPosition;
}

// Returns Blinky's current position
Vector2 Blinky::getPosition() {
  return position;
}
