#include "Entity.h"

// Constructor: Initializes entity position, radius, and speed
Entity::Entity(Vector2 startPos, float radius, float speed) : position(startPos), radius(radius), speed(speed) {}

// Draws the entity as a circle (default)
void Entity::draw(Color colour) {
  DrawCircleV(position, radius, colour);
}

// Default update does nothing (to be overridden by derived classes)
void Entity::update(){}

// Returns the current position of the entity
Vector2 Entity::getPosition() {
  return position;
}

// Returns the x coordinate of the entity
float Entity::getX() {
  return position.x;
}

// Returns the y coordinate of the entity
float Entity::getY() {
  return position.y;
}
