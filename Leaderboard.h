#pragma once

#include <time.h>
#include <string>
#include <vector>

struct Score {
  std::string name;
  int points;
  time_t time;
};

class Leaderboard {
  Score currentScore;
  std::vector<Score> scores;
  void sortAndTrim();

  public:
    Leaderboard();
    void loadBoard(const std::string& filename);
    void saveBoard(const std::string& filename);
    void addScore(Score score);
    void printLeaderboard();
    void clearBoard();
    const std::vector<Score>& getScores();
    Score& getCurrentScore();
    Score* pGetCurrentScore();
  };
