#ifndef PLAYFIELD_H
#define PLAYFIELD_H
#include <vector>
#include "tetromino.h"

class Playfield {
private:
    int rows;
    int cols;
    std::vector<std::vector<int>> grid;

public:
    Playfield(int rows, int cols);
    
    void reset();
    void setCell(int r, int c, int value);
    int getCell(int r, int c) const;
    
    bool isValidPosition(const Tetromino& t, int dRow, int dCol) const;
    void placeTetromino(const Tetromino& t);
    int clearFullLines();
    
    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

#endif