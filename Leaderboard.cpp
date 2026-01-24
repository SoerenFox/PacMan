#include "Leaderboard.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip> 

// Constructor: Initializes the leaderboard and sets default player name
Leaderboard::Leaderboard(){ currentScore.name = "Player"; }

// Loads the leaderboard from a file
void Leaderboard::loadBoard(const std::string& filename){
  scores.clear();
  std::ifstream file(filename);
  if (!file.is_open()){
    std::cerr << "Error opening '" << filename << "'. May occure if file doesn't exist yet" << std::endl;
    return;
  }
  std::string line;
  while (std::getline(file, line)){
    std::istringstream iss(line);
    std::string name;
    int points;
    time_t winTime;
    if (iss >> name >> points >> winTime){
      scores.push_back({name, points, winTime});
    } else {
      std::cerr << "Error reading line: " << line << std::endl;
    }
  }
  sortAndTrim();
}

// Saves the leaderboard to a file
void Leaderboard::saveBoard(const std::string& filename){
  std::ofstream file(filename);
  if (!file.is_open()){
    std::cerr << "Error opening '" << filename << "' for writing." << std::endl;
    return;
  }
  for (const auto& entry : scores){
    file << entry.name << '\t' << entry.points << '\t' << static_cast<long>(entry.time) << '\n';
  }
}

// Adds a new score to the leaderboard and sorts/trims
void Leaderboard::addScore(Score score){
  scores.push_back(score);
  sortAndTrim();
}

// Sorts the leaderboard and keeps only the top 10 scores
void Leaderboard::sortAndTrim(){
  // Sort descending by points, then ascending by time, unfinished games last
  std::sort(scores.begin(), scores.end(), [](const Score& a, const Score& b) {
    if (a.points != b.points)
      return a.points > b.points; // higher points first
    if (a.time == -1 && b.time != -1)
      return false; // a is unfinished, b is finished -> b before a
    if (a.time != -1 && b.time == -1)
      return true; // a is finished, b is unfinished -> a before b
    return a.time < b.time; // both finished, faster time first
  });
  // Keep only top 10
  if (scores.size() > 10) scores.resize(10);
}

// Prints the leaderboard to the console
void Leaderboard::printLeaderboard(){
  std::cout << "Leaderboard:\n";
  std::cout << "Rank\tPoints\tTime (s)\n";
  int rank = 1;
  for (const auto& entry : scores) {
    std::cout << rank << '\t' << entry.points << '\t';
    if (entry.time == -1) {
      std::cout << "-";
    } else {
      std::cout << std::fixed << std::setprecision(3) << (entry.time / 1000.0);
    }
    std::cout << '\n';
    ++rank;
  }
}

// Clears all scores from the leaderboard
void Leaderboard::clearBoard(){
  scores.clear();
}

// Returns a const reference to the scores vector
const std::vector<Score>& Leaderboard::getScores(){
  return scores;
}

Score& Leaderboard::getCurrentScore(){
  return currentScore;
}

Score* Leaderboard::pGetCurrentScore(){
  return &currentScore;
}