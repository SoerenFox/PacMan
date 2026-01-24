#include "Display.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "Pacman.h"
#include "Audio.h"

#include "raylib.h"
#include <iostream>
#include <string>

// Constructor: Initializes the Display class and its sounds
Display::Display(){}

// Draws a button at the given position, handles mouse and key interaction, and updates the game state if pressed
void Display::button(GameState& gameState, GameState setState, float y, const char* title, int pressedKey){
  if(IsKeyPressed(pressedKey)) gameState = setState;

  Rectangle button = {140 * 1.4, y, 420 * 1.4, 60 * 1.4};
  int fontSize = 32 * 1.4, textWidth = MeasureText(title, fontSize);
  Color buttonColor = CheckCollisionPointRec(GetMousePosition(), button) ? WHITE : GRAY;

  DrawRectangleRoundedLines(button, 0.3f * 1.4, 10 * 1.4, 2 * 1.4, buttonColor);
  DrawText(title, button.x + (button.width - textWidth) / 2, button.y + (button.height - fontSize) / 2, fontSize, buttonColor);

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), button)) {
    // Button was clicked
    gameState = setState;
  }
}

// Draws a volume slider and allows the user to adjust the audio volume interactively
void Display::soundSlider(float sliderY, const char* label) {
  float currentVolume = audio.getVolume();
  float sliderX = 150 * 1.4, sliderW = 320 * 1.4, sliderH = 12 * 1.4;
  float knobRadius = 18 * 1.4;
  sliderY += 38 * 1.4;

  DrawRectangle(sliderX, sliderY, sliderW, sliderH, GRAY);
  DrawRectangle(sliderX, sliderY, sliderW * currentVolume, sliderH, SKYBLUE);
  float knobX = sliderX + sliderW * currentVolume;
  DrawCircle(knobX, sliderY + sliderH / 2, knobRadius, WHITE);
  DrawCircleLines(knobX, sliderY + sliderH / 2, knobRadius, DARKGRAY);

  Vector2 mouse = GetMousePosition();
  Rectangle knobRect = { knobX - knobRadius, sliderY + sliderH / 2 - knobRadius, knobRadius * 2, knobRadius * 2 };
  static bool dragging = false;

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointCircle(mouse, {knobX, sliderY + sliderH / 2}, knobRadius)) {
    dragging = true;
  }
  if (dragging) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      float newVolume = (mouse.x - sliderX) / sliderW;
      if (newVolume < 0) newVolume = 0;
      if (newVolume > 1) newVolume = 1;
      audio.setVolume(newVolume);
    } else {
      dragging = false;
    }
  }

  DrawText(label, sliderX, sliderY - 40 * 1.4, 28 * 1.4, WHITE);
  char buf[16];
  snprintf(buf, sizeof(buf), "%d%%", int(currentVolume * 100));
  DrawText(buf, sliderX + sliderW + 30, sliderY + sliderH / 2 - 16 * 1.4, 28 * 1.4, WHITE);
}

// Draws a button for changing the player's name and handles text input and cursor blinking
void Display::nameButton(float y, const char* title, Score& currentScore) {
  Rectangle buttonRect = {140 * 1.4, y, 420 * 1.4, 60 * 1.4};
  int fontsize = 32 * 1.4, textWidth = MeasureText(title, fontsize);
  Color buttonColor = CheckCollisionPointRec(GetMousePosition(), buttonRect) ? WHITE : GRAY;

  DrawRectangleRoundedLines(buttonRect, 0.3f * 1.4, 10 * 1.4, 2 * 1.4, buttonColor);
  DrawText(title, buttonRect.x + (buttonRect.width - textWidth) / 2, buttonRect.y + (buttonRect.height - fontsize) / 2, fontsize, buttonColor);

  static bool editing = false;
  static char nameBuf[32] = "";
  static float cursorTimer = 0.0f;
  static bool cursorVisible = true;

  if (!editing) strncpy(nameBuf, currentScore.name.c_str(), sizeof(nameBuf)-1);
  if (!editing && CheckCollisionPointRec(GetMousePosition(), buttonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    editing = true;
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
  }

  if (editing) {
    // Handle character input
    int key = GetCharPressed();
    while (key > 0) {
      int len = (int)strlen(nameBuf);
      if (key >= 32 && key <= 126 && len < 30) {
        nameBuf[len] = (char)key;
        nameBuf[len + 1] = '\0';
      }
      key = GetCharPressed();
  }
  // Handle backspace
  if (IsKeyPressed(KEY_BACKSPACE)) {
    int len = (int)strlen(nameBuf);
    if (len > 0) nameBuf[len - 1] = '\0';
  }
  // Handle enter (finish editing)
  if (IsKeyPressed(KEY_ENTER)) {
    currentScore.name = nameBuf;
    editing = false;
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }
  // Exit editing if clicked outside
  if (!CheckCollisionPointRec(GetMousePosition(), buttonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    editing = false;
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
  }

  // Blinking cursor
  cursorTimer += GetFrameTime();
  if (cursorTimer >= 0.5f) {
    cursorVisible = !cursorVisible;
    cursorTimer = 0.0f;
  }
  char displayBuf[36];
  snprintf(displayBuf, sizeof(displayBuf), "%s%s", nameBuf, (cursorVisible ? "|" : ""));
  DrawText(displayBuf, 48 * 21 / 2 - MeasureText(displayBuf, 28 * 1.4) / 2, 142 * 1.4, 28 * 1.4, WHITE);
  } 
}

// Draws a mute/unmute button and toggles the audio volume between 0 and 0.5
void Display::muteButton(float y, const char* title){
  Rectangle buttonRect = {140 * 1.4, y, 420 * 1.4, 60 * 1.4};
  int fontsize = 32 * 1.4, textWidth = MeasureText(title, fontsize);
  Color buttonColor = CheckCollisionPointRec(GetMousePosition(), buttonRect) ? WHITE : GRAY;

  DrawRectangleRoundedLines(buttonRect, 0.3f * 1.4, 10 * 1.4, 2 * 1.4, buttonColor);
  DrawText(title, buttonRect.x + (buttonRect.width - textWidth) / 2, buttonRect.y + (buttonRect.height - fontsize) / 2, fontsize, buttonColor);

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), buttonRect) || IsKeyPressed(KEY_M)){
    if (audio.getVolume() == 0){
      audio.setVolume(0.5f);
    } else {
      audio.setVolume(0.0f);
    }
  }
}

// Shows the start menu, draws menu buttons, and handles menu navigation and hover effects
void Display::startMenu(GameState& gameState){
  if (!IsSoundReady(startSound)) startSound = LoadSound("sounds/start.mp3");
  SetSoundVolume(startSound, audio.getVolume());
  PlaySound(startSound);
  while((gameState == GameState::MENU) && !WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int fontsize = 48 * 1.4;
    const char* defaultTitle = "Pac-Man";
    const char* ghostNames[4] = { "Clyde", "Inky", "Pinky", "Blinky" };
    Color ghostColors[4] = { ORANGE, SKYBLUE, PINK, RED };
    Color pacmanColor = YELLOW;

    int height = 270 * 1.4;
    int radius = 18 * 1.4;
    int ghostSpacing = 12 * 1.4;
    int pacmanToGhostGap = 60 * 1.4;
    int yPos = height - 80 * 1.4;
    int xMax = (radius * 2 * 4 + pacmanToGhostGap + ghostSpacing * 3) / 2;

    // Calculate ghost positions
    float ghostCenters[4];
    for (int i = 0; i < 4; ++i) {
      ghostCenters[i] = 360 * 1.4f + xMax - (ghostSpacing + radius * 2) * i;
    }

    // Check mouse hover over ghosts and Pac-Man
    Vector2 mouse = GetMousePosition();
    const char* title = defaultTitle;
    Color titleColor = WHITE;
    bool pacmanHovered = false;
    // Check Pac-Man hover
    float dx = mouse.x - (360 * 1.4f - xMax);
    float dy = mouse.y - yPos;
    if (dx * dx + dy * dy <= radius * radius) {
      title = defaultTitle;
      titleColor = pacmanColor;
      pacmanHovered = true;
    } else {
      for (int i = 0; i < 4; ++i) {
        float dxg = mouse.x - ghostCenters[i];
        float dyg = mouse.y - yPos;
        if (dxg * dxg + dyg * dyg <= radius * radius) {
          title = ghostNames[i];
          titleColor = ghostColors[i];
          break;
        }
      }
    }

    DrawText(title, 360 * 1.4f - MeasureText(title, fontsize) / 2, 60 * 1.4f, fontsize, titleColor);

    //DrawCircle(360 * 1.4f - xMax, yPos, radius, YELLOW);
    button(gameState, GameState::PLAYING, height, "Play", KeyboardKey::KEY_ENTER);
    button(gameState, GameState::LEADERBOARD, height += 80 * 1.4, "Leaderboard", KeyboardKey::KEY_L);
    button(gameState, GameState::SETTINGS, height += 80 * 1.4, "Settings", KeyboardKey::KEY_S);
    button(gameState, GameState::QUIT, height += 140 * 1.4, "Quit", KeyboardKey::KEY_Q);

    Pacman pacman({static_cast<float>(360 * 1.4f - xMax), static_cast<float>(yPos)}, static_cast<float>(radius), 0.0f);
    pacmanHovered ? pacman.draw() : DrawCircleSector({360 * 1.4f - xMax, (float)yPos}, radius, 20, 340, 60, YELLOW);

    // Draw ghosts using their draw() methods, but keep them stationary and facing left
    Clyde clyde({static_cast<float>(ghostCenters[0]), static_cast<float>(yPos)}, static_cast<float>(radius), 0.0f);
    Inky inky({static_cast<float>(ghostCenters[1]), static_cast<float>(yPos)}, static_cast<float>(radius), 0.0f);
    Pinky pinky({static_cast<float>(ghostCenters[2]), static_cast<float>(yPos)}, static_cast<float>(radius), 0.0f);
    Blinky blinky({static_cast<float>(ghostCenters[3]), static_cast<float>(yPos)}, static_cast<float>(radius), 0.0f);
    clyde.setCurrentDirection(Direction::LEFT);
    inky.setCurrentDirection(Direction::LEFT);
    pinky.setCurrentDirection(Direction::LEFT);
    blinky.setCurrentDirection(Direction::LEFT);
    clyde.draw();
    inky.draw();
    pinky.draw();
    blinky.draw();

    EndDrawing();
  }
}

// Displays the leaderboard with top scores and allows returning to the menu
void Display::leaderboard(GameState& gameState, const std::vector<Score>& scores) {
  while((gameState == GameState::LEADERBOARD) && !WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int fontsize = 20 * 1.4, entries = static_cast<int>(scores.size());
    float height = 142 * 1.4, rowHeight = 38 * 1.4;

    DrawText("Leaderboard", 360 * 1.4 - MeasureText("Leaderboard", 48 * 1.4) / 2, 60 * 1.4, 48 * 1.4, WHITE);
    DrawText("Rank", (360 * 1.4 - (600 * 1.4) / 2) + (90 * 1.4 - MeasureText("Rank", fontsize)) / 2, height + (rowHeight - fontsize) / 2, fontsize, WHITE);
    DrawText("Name", (360 * 1.4 - (600 * 1.4) / 2 + 90 * 1.4) + (170 * 1.4 - MeasureText("Name", fontsize)) / 2, height + (rowHeight - fontsize) / 2, fontsize, WHITE);
    DrawText("Points", (360 * 1.4 - (600 * 1.4) / 2 + 90 * 1.4 + 170 * 1.4) + (170 * 1.4 - MeasureText("Points", fontsize)) / 2, height + (rowHeight - fontsize) / 2, fontsize, WHITE);
    DrawText("Time [s]", (360 * 1.4 - (600 * 1.4) / 2 + 90 * 1.4 + 170 * 1.4 + 170 * 1.4) + (170 * 1.4 - MeasureText("Time [s]", fontsize)) / 2, height + (rowHeight - fontsize) / 2, fontsize, WHITE);

    for (int i = 0; i < entries; ++i) {
      DrawRectangle(360 * 1.4 - (600 * 1.4) / 2, 180 * 1.4 + i * rowHeight, 600 * 1.4, rowHeight, Fade((i % 2 == 0) ? DARKGRAY : GRAY, 0.25f));

      char buf[32];
      snprintf(buf, sizeof(buf), "%d", i + 1);
      DrawText(buf, (360 * 1.4 - (600 * 1.4) / 2) + (90 * 1.4 - MeasureText(buf, fontsize)) / 2, 180 * 1.4 + i * rowHeight + (rowHeight - fontsize) / 2, fontsize, WHITE);

      DrawText(scores[i].name.c_str(), (360 * 1.4 - (600 * 1.4) / 2 + 90 * 1.4) + (170 * 1.4 - MeasureText(scores[i].name.c_str(), fontsize)) / 2, 180 * 1.4 + i * rowHeight + (rowHeight - fontsize) / 2, fontsize, WHITE);

      if (scores[i].name.empty()){
        snprintf(buf, sizeof(buf), "-");
      } else {
        snprintf(buf, sizeof(buf), "%d", scores[i].points);
      }
      DrawText(buf, (360 * 1.4 - (600 * 1.4) / 2 + 90 * 1.4 + 170 * 1.4) + (170 * 1.4 - MeasureText(buf, fontsize)) / 2, 180 * 1.4 + i * rowHeight + (rowHeight - fontsize) / 2, fontsize, WHITE);

      if (scores[i].time != -1) {
        snprintf(buf, sizeof(buf), "%0.3f", float(scores[i].time) / 1000.0f);
      } else {
        snprintf(buf, sizeof(buf), "-");
      }
      DrawText(buf, (360 * 1.4 - (600 * 1.4) / 2 + 90 * 1.4 + 170 * 1.4 + 170 * 1.4) + (170 * 1.4 - MeasureText(buf, fontsize)) / 2, 180 * 1.4 + i * rowHeight + (rowHeight - fontsize) / 2, fontsize, WHITE);
    }
    DrawRectangleRoundedLines({ 360 * 1.4 - (600 * 1.4) / 2, height, 600 * 1.4, (rowHeight * entries + rowHeight) }, 0.08f, 8, 3, PURPLE);
    DrawRectangleRoundedLines({ 360 * 1.4 - (600 * 1.4) / 2 - 2, (180 - 40) * 1.4, 600 * 1.4 + 4, (rowHeight * entries + 59) }, 0.08f, 8, 5, BLACK);
    button(gameState, GameState::MENU, 570 * 1.4, "Back to Menu", KeyboardKey::KEY_B);

    EndDrawing();
  }
}

// Draws the end screen (win/lose), displays the final score, and provides options to restart or return to menu
void Display::drawEnd(GameState& gameState, const char* title, const Score score){
  if (!IsSoundReady(winSound)) winSound = LoadSound("sounds/take_on_me_small.mp3");
  if (!IsSoundReady(deathSound)) deathSound = LoadSound("sounds/death.mp3");
  if (gameState == GameState::WON) {
    SetSoundVolume(winSound, audio.getVolume());
    PlaySound(winSound);
  } else if (gameState == GameState::LOST) {
    SetSoundVolume(deathSound, audio.getVolume());
    PlaySound(deathSound);
  }
  while((gameState == GameState::WON || gameState == GameState::LOST) && !WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    int startheight = 270 * 1.4;

    // Center the two columns within the rectangle
    float rectX = 120 * 1.4f;
    float rectY = 180 * 1.4f;
    float rectW = 480 * 1.4f;
    float rectH = 38 * 1.4f;
    float colW = rectW / 2.0f;
    float labelFont = 20 * 1.4f;
    float valueFont = 20 * 1.4f;

    // Draw column labels centered
    DrawText("Points", rectX + (colW - MeasureText("Points", labelFont)) / 2, 142 * 1.4f + (rectH - labelFont) / 2, labelFont, WHITE);
    DrawText("Time", rectX + colW + (colW - MeasureText("Time", labelFont)) / 2, 142 * 1.4f + (rectH - labelFont) / 2, labelFont, WHITE);

    DrawRectangle(rectX, rectY, rectW, rectH, Fade(DARKGRAY, 0.25f));
    DrawRectangleRoundedLines({ rectX, 142 * 1.4f, rectW, rectH * 2 }, 0.08f, 8, 3, BLUE);
    DrawRectangleRoundedLines({ rectX - 2 * 1.4f, 140 * 1.4f, rectW + 4 * 1.4f, rectH + 42 * 1.4f }, 0.08f, 8, 5, BLACK);

    // Draw points value centered in left column
    char pointsBuf[16];
    snprintf(pointsBuf, sizeof(pointsBuf), "%d", score.points);
    DrawText(pointsBuf, rectX + (colW - MeasureText(pointsBuf, valueFont)) / 2, rectY + (rectH - valueFont) / 2, valueFont, WHITE);

    // Draw time value centered in right column
    char timeBuf[16];
    snprintf(timeBuf, sizeof(timeBuf), score.time != -1 ? "%0.3f" : "-", float(score.time / 1000.0));
    DrawText(timeBuf, rectX + colW + (colW - MeasureText(timeBuf, valueFont)) / 2, rectY + (rectH - valueFont) / 2, valueFont, WHITE);

    DrawText(title, 360 * 1.4 - MeasureText(title, 48 * 1.4) / 2, 60 * 1.4, 48 * 1.4, WHITE);

    button(gameState, GameState::PLAYING, startheight, "Restart", KeyboardKey::KEY_ENTER);
    button(gameState, GameState::PLAYING, startheight, "Restart", KeyboardKey::KEY_R);
    button(gameState, GameState::LEADERBOARD, startheight += 80 * 1.4, "Leaderboard", KeyboardKey::KEY_L);
    button(gameState, GameState::MENU, startheight += 80 * 1.4, "Back to Menu", KeyboardKey::KEY_B);
    button(gameState, GameState::QUIT, startheight += 160 * 1.4, "Quit", KeyboardKey::KEY_Q);
    EndDrawing();
  }
}

// Shows the settings menu, including volume slider, mute button, and name change
void Display::settings(GameState& gameState, Score& score){
  while((gameState == GameState::SETTINGS) && !WindowShouldClose()) {
    int height = 270 * 1.4;

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Settings", 360 * 1.4 - MeasureText("Settings", 48 * 1.4) / 2, 60 * 1.4, 48 * 1.4, WHITE);

    soundSlider(height, "Volume");
    muteButton(height += 80 * 1.4, "Mute");
    nameButton(height += 80 * 1.4, "Change Name", score);
    button(gameState, GameState::MENU, height += 140 * 1.4, "Back to Menu", KeyboardKey::KEY_B);
    EndDrawing();
  }
}