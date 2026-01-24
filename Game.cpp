#include "Game.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

// Game constructor: initializes map, pacman, ghosts, audio, and random seed
Game::Game() : pacman({ map.tileSize * 10 + map.tileSize / 2.0f, map.tileSize * 14 + map.tileSize / 2.0f }, 18.0f, 4.0f){
  Vector2 startPosGhosts = { map.tileSize * 10 + map.tileSize / 2.0f, map.tileSize * 7 + map.tileSize / 2.0f };
  ghosts.push_back(new Blinky(startPosGhosts, 16.0f, 4.0f));
  ghosts.push_back(new Pinky(startPosGhosts, 16.0f, 4.0f));
  ghosts.push_back(new Inky(startPosGhosts, 16.0f, 4.0f));
  ghosts.push_back(new Clyde(startPosGhosts, 16.0f, 4.0f));       
  
  audio.setVolume(0.5f); // Set default volume
  float volume = audio.getVolume(); // Initialize volume
  srand(static_cast<unsigned int>(time(nullptr))); // random seed
}

// Checks for end conditions: collision with ghosts or all coins collected
void Game::checkEnd(Score &currentScore){
  // Check for collision with ghosts
  for (int i = 0; i < ghosts.size(); ++i){
    if (int(pacman.getX() / map.tileSize) == int(ghosts[i]->getX() / map.tileSize) && int(pacman.getY() / map.tileSize) == int(ghosts[i]->getY() / map.tileSize)){
      gameState = GameState::LOST;
      currentScore.time = -1; // indicate no value / unset
      return;
    }
  }

  // Check for all coins collected 
  bool coinsLeft = false;
  for (int x = 0; x < map.tileCount && !coinsLeft; x++){
    for (int y = 0; y < map.tileCount && !coinsLeft; y++){
      if (map.map[y][x] == static_cast<int>(TileType::Coin)){
        coinsLeft = true;
      }
    }
  }
  if (!coinsLeft){
    gameState = GameState::WON;
    currentScore.time = static_cast<int>((double(clock() - currentScore.time) / CLOCKS_PER_SEC) * 1000); // ms
    return;
  }
}

// Main game loop: updates entities, checks for end, draws everything
void Game::gameLoop(Score &currentScore){
  currentScore.points = 0;
  currentScore.time = clock();
  while (gameState == GameState::PLAYING && !WindowShouldClose()){
    pacman.update(map.map, map.tileSize, currentScore);
    for (int i = 0; i < ghosts.size(); ++i) ghosts[i]->update(ghosts[i]->getTarget(pacman.getPosition(), pacman.getCurrentDirection(), ghosts[0]->getPosition()), map.map, map.tileSize);
    
    checkEnd(currentScore);
    
    BeginDrawing();
    ClearBackground(BLACK);
    map.draw();
    pacman.draw();

    Color ghostColors[4] = { RED, PINK, BLUE, ORANGE };
    for (int i = 0; i < ghosts.size(); ++i) ghosts[i]->draw();
    
    map.spawnFruit();

    EndDrawing();
  }
}

// Resets the game state for a new round
void Game::reset(){
  for (auto ghost : ghosts) delete ghost;
  ghosts.clear();
  map.reset(); 
  pacman = Pacman({ map.tileSize * 10 + map.tileSize / 2.0f, map.tileSize * 14 + map.tileSize / 2.0f }, 18.0f, 4.0f);

  Vector2 startPosGhosts = { map.tileSize * 10 + map.tileSize / 2.0f, map.tileSize * 7 + map.tileSize / 2.0f };
  ghosts.push_back(new Blinky(startPosGhosts, 16.0f, 4.0f));
  ghosts.push_back(new Pinky(startPosGhosts, 16.0f, 4.0f));
  ghosts.push_back(new Inky(startPosGhosts, 16.0f, 4.0f));
  ghosts.push_back(new Clyde(startPosGhosts, 16.0f, 4.0f));
}

// Runs the main menu and handles state transitions
void Game::run(){
  leaderboard.loadBoard(LEADERBOARD_PATH);

  //leaderboard.clearBoard();
  
  InitWindow(map.screenHeight, map.screenHeight, "Pacman");
  InitAudioDevice();
  SetTargetFPS(60);
  gameState = GameState::MENU;
  
  while (!WindowShouldClose()){
    switch (gameState){
      case GameState::MENU:
        display.startMenu(gameState);
        break;
      case GameState::SETTINGS:
        display.settings(gameState, *leaderboard.pGetCurrentScore());
        break;
      case GameState::LEADERBOARD:
        display.leaderboard(gameState, leaderboard.getScores());
        break;
      case GameState::PLAYING:
        gameLoop(leaderboard.getCurrentScore());
        leaderboard.addScore(leaderboard.getCurrentScore());
        leaderboard.saveBoard(LEADERBOARD_PATH);
        reset();
        break;
      case GameState::WON:
        display.drawEnd(gameState, "You Won!", leaderboard.getCurrentScore());
        break;
      case GameState::LOST:
        display.drawEnd(gameState, "You Lost!", leaderboard.getCurrentScore()); 
        break;
      case GameState::QUIT:
        CloseWindow();
    }
  }
}