#pragma once

#include "raylib.h"

class Audio {
  
  public:
    static float volume;
    Audio();
    void setVolume(float volume);
    float getVolume() const;
};