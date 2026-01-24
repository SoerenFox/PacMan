#pragma once

#include "GameState.h"
#include "Leaderboard.h"
#include "Audio.h"

#include <string>

class Display {
  
  Audio audio;
  Sound deathSound = { 0 };
  Sound winSound = { 0 };
  Sound startSound = { 0 };

  void button(GameState &gameState, GameState setState, float y, const char* title, int pressedKey);
  void soundSlider(float sliderY, const char* label);
  void nameButton(float y, const char* title, Score& currentScore);
  void muteButton(float y, const char* title);

  public:
    Display();
    void startMenu(GameState& gameState);
    void settings(GameState& gameState, Score& score);
    void leaderboard(GameState& gameState, const std::vector<Score>& scores);
    void drawEnd(GameState& gameState, const char* title, const Score score);   
};
