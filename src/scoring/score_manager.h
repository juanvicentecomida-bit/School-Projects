#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

class ScoreManager {
private:
    int score;
    int totalLines;
    int level;

public:
    ScoreManager();
    
    void addLineScore(int linesCleared);
    void addDropBonus(int cells, bool isHardDrop);
    void reset();
    void setLevel(int startLevel);
    
    int getScore() const { return score; }
    int getLines() const { return totalLines; }
    int getLevel() const { return level; }
    
    int calculateDropSpeed() const;

private:
    void updateLevel();
    int getLineBaseScore(int linesCleared) const;
};

#endif