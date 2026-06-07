#ifndef TETROMINO_H
#define TETROMINO_H
#include <vector>
#include <queue>

enum class TetrominoType { I, O, T, S, Z, J, L };

class Tetromino {
private:
    TetrominoType type;
    std::vector<std::vector<int>> shape;
    int row, col;
    int rotationState;
    
    void initializeShape();

public:
    Tetromino(TetrominoType t);
    
    void rotateCW();
    void rotateCCW();
    void moveDown();
    void moveLeft();
    void moveRight();

    void setPosition(int r, int c) { row = r; col = c; }
    
    const std::vector<std::vector<int>>& getShape() const;
    int getRow() const;
    int getCol() const;
    TetrominoType getType() const;
    int getRotationState() const { return rotationState; }
    
    static TetrominoType getRandomType();
};

class TetrominoBag {
private:
    std::queue<TetrominoType> bag;
    unsigned int seed;
    
    void refillBag();
    int customRand();
    
public:
    TetrominoBag(unsigned int s = 0);
    TetrominoType getNext();
};

#endif