#include "KeyPoint.h"


KeyPoint::KeyPoint(){

}

KeyPoint::~KeyPoint(){

}

KeyPoint::KeyPoint(const KeyPoint &other){
    if(this != &other){
        *this = other;
    }
}

KeyPoint &KeyPoint::operator=(const KeyPoint &other){
    if(this != &other){
        x = other.x;
        y = other.y;
        sigma = other.sigma;
        angle = other.angle;
    }
    return *this;
}

KeyPoint::KeyPoint(int xin, int yin, float sigmaIn, float angleIn){
    x = xin;
    y = yin;
    sigma = sigmaIn;
    angle = angleIn;
}

void KeyPoint::Dimensions(
    int windowSize, 
    int &outX, 
    int &outY, 
    int &outEndX, 
    int &outEndY
){
    int radius = windowSize * sigma; 
    outX = x - radius;
    outY = y - radius;
    outEndX = x + radius;
    outEndY = y + radius;
}