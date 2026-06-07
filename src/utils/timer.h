#ifndef TIMER_H
#define TIMER_H

class Timer {
private:
    int frameCounter;
    int targetFrames;

public:
    Timer(int target = 0) : frameCounter(0), targetFrames(target) {}
    
    void tick() { frameCounter++; }
    void reset() { frameCounter = 0; }
    void setTarget(int target) { targetFrames = target; }
    bool isReady() const { return frameCounter >= targetFrames; }
    int getFrames() const { return frameCounter; }
};

#endif