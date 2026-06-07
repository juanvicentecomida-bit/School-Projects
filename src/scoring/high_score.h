#ifndef HIGH_SCORE_H
#define HIGH_SCORE_H

#include <string>
#include <vector>

struct ScoreEntry {
    std::string playerName;
    int score;
    int lines;
    int level;
    std::string date;
    
    ScoreEntry() : playerName(""), score(0), lines(0), level(0), date("") {}
    ScoreEntry(const std::string& name, int s, int l, int lv, const std::string& d)
        : playerName(name), score(s), lines(l), level(lv), date(d) {}
};

class HighScoreManager {
private:
    std::string filepath;
    std::vector<ScoreEntry> scores;
    int maxEntries;
    
    std::string getCurrentDate() const;
    void sortScores();

public:
    HighScoreManager(const std::string& path = "scores.txt", int maxScores = 10);
    
    bool loadScores();
    bool saveScores() const;
    
    bool addScore(const std::string& playerName, int score, int lines, int level);
    
    bool isHighScore(int score) const;
    
    const std::vector<ScoreEntry>& getScores() const { return scores; }
    int getHighestScore() const;
    
    void displayScores() const;
};

#endif