#include "FlipTimer.h"

FlipTimer::FlipTimer() : Timer() {

};

FlipTimer::~FlipTimer(){
    
}

bool FlipTimer::timesUp(){
    if(Timer::timesUp()){
        bFlipped = !bFlipped;
        return true;
    }
    return false;
}

float FlipTimer::scalar(){
    float scalarRaw = Timer::scalar();
    if(bFlipped){
        scalarRaw = 1.0f - scalarRaw;
    }
    return scalarRaw;
}