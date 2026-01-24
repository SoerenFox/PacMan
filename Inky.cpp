#include "Inky.h"

// Constructor: Initializes Inky with position, radius, and speed
Inky::Inky(Vector2 startPos, float radius, float speed) : Ghost(startPos, radius, speed, SKYBLUE) {}

// Returns the target position for Inky (uses Pacman and Blinky positions)
Vector2 Inky::getTarget(Vector2 pacmanPosition, Direction direction, Vector2 blinkyPosition) {
  blinkyPosition.x -= pacmanPosition.x;
  blinkyPosition.y -= pacmanPosition.y;

  switch (direction) {
    case Direction::UP:
      pacmanPosition.x = pacmanPosition.x;
      pacmanPosition.y -= (48);
      break;
    case Direction::DOWN:
      pacmanPosition.x = pacmanPosition.x;
      pacmanPosition.y += (48);
      break;
    case Direction::LEFT:
      pacmanPosition.x -= (48);
      pacmanPosition.y = pacmanPosition.y;
      break;
    case Direction::RIGHT:
      pacmanPosition.x += (48);
      pacmanPosition.y += pacmanPosition.y;
      break;
    default:
      return pacmanPosition; 
          
  }
  pacmanPosition.x += blinkyPosition.x;
  pacmanPosition.y -= blinkyPosition.y;

  return { pacmanPosition.x, pacmanPosition.y };
}