#ifndef WALL_KICKS_H
#define WALL_KICKS_H

#include <vector>
#include <utility>

struct KickOffset {
    int row;
    int col;
    
    KickOffset(int r, int c) : row(r), col(c) {}
};

class WallKicks {
public:
    static std::vector<KickOffset> getNormalKicks(int fromRotation, int toRotation) {
        std::vector<KickOffset> kicks;
        
        kicks.push_back(KickOffset(0, 0));   
        kicks.push_back(KickOffset(0, -1)); 
        kicks.push_back(KickOffset(0, 1));   
        kicks.push_back(KickOffset(-1, 0));  
        kicks.push_back(KickOffset(1, 0));   
        
        kicks.push_back(KickOffset(0, -2));  
        kicks.push_back(KickOffset(0, 2));   
        
        return kicks;
    }
    
    static std::vector<KickOffset> getIPieceKicks(int fromRotation, int toRotation) {
        std::vector<KickOffset> kicks;
        
        kicks.push_back(KickOffset(0, 0));   
        kicks.push_back(KickOffset(0, -1));  
        kicks.push_back(KickOffset(0, 1));   
        kicks.push_back(KickOffset(0, -2));  
        kicks.push_back(KickOffset(0, 2));   
        kicks.push_back(KickOffset(-1, 0)); 
        kicks.push_back(KickOffset(1, 0));   
    
        if ((fromRotation == 0 && toRotation == 1) || 
            (fromRotation == 1 && toRotation == 0)) {
            kicks.push_back(KickOffset(0, -3));  
            kicks.push_back(KickOffset(0, 3));   
        }
        
        return kicks;
    }
    
    static std::vector<KickOffset> getOPieceKicks(int fromRotation, int toRotation) {
        std::vector<KickOffset> kicks;
        kicks.push_back(KickOffset(0, 0));
        return kicks;
    }
};

#endif