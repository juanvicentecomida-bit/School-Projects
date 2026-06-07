#ifndef GHOST_PIECE_H
#define GHOST_PIECE_H

#include "tetromino.h"
#include "playfield.h"

class GhostPiece {
public:
    static int calculateLandingRow(const Tetromino& piece, const Playfield& field) {
        Tetromino testPiece = piece;
        
        while (field.isValidPosition(testPiece, 1, 0)) {
            testPiece.moveDown();
        }
        
        return testPiece.getRow();
    }
    
    static bool isGhostCell(const Tetromino& piece, const Playfield& field, int row, int col) {
        int landingRow = calculateLandingRow(piece, field);
        
        if (landingRow == piece.getRow()) {
            return false;
        }
        
        const auto& shape = piece.getShape();
        int relativeRow = row - landingRow;
        int relativeCol = col - piece.getCol();
        
        if (relativeRow >= 0 && relativeRow < shape.size() &&
            relativeCol >= 0 && relativeCol < shape[0].size()) {
            return shape[relativeRow][relativeCol] != 0;
        }
        
        return false;
    }
};

#endif