#include "Audio.h"
#include "raylib.h"

// Static member initialization for default volume
float Audio::volume = 0.5f;

// Constructor: Sets the master volume to the current value
Audio::Audio() {
  setVolume(volume);
}

// Sets the master volume to the given value
void Audio::setVolume(float newVolume) {
  volume = newVolume;
  SetMasterVolume(volume);
}

// Returns the current master volume
float Audio::getVolume() const {
  return volume;
}


