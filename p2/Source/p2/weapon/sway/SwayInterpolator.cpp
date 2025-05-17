#include "SwayInterpolator.h"

SwayInterpolator::SwayInterpolator(){

}

SwayInterpolator::~SwayInterpolator(){

}

void SwayInterpolator::setMotionTime(float time){
    time = std::abs(time);
    fullMotionTime = time;
    /*
    gesucht x wenn full also sin(2pi):

    sin(time / x) = sin(2 * pi)    //asin
    time / x = 2*pi                // * time
    time = 2 * pi * x
    time / (2 * pi) = x
     
    */

    float pi2 = M_PI * 2.0f;
    float x = time / pi2;

    fullMotionStretchSinWaveDenominator = x;

}

void SwayInterpolator::setHeightOfWave(float y, float z){
    heightY = y;
    heightZ = z;
}


void SwayInterpolator::enableSwayFlag(bool flag){
    
    if(flag){
		swayEnabled = true;
		stopSway = false;
	}else{
		if(!swayEnabled || stopSway){
			return;
		}
		stopSway = true;

        //interpolate to end or to start
        float distToStart = deltaTimeSum;
        float distToEnd = fullMotionTime - deltaTimeSum;

        timeSumSign = (distToStart < distToEnd) ? -1.0f : 1.0f;
	}
    
}


FVector SwayInterpolator::Tick(float deltatime){
    if(DEBUG_BLOCKSWAY || !swayEnabled){
        return FVector(0,0,0);
    }
    
    //integral sin dt
    deltaTimeSum += deltatime * timeSumSign; 

    if(deltaTimeSum > fullMotionTime || deltaTimeSum < 0.0f){
        reachedPeakFlag = true;
    }

    //disable
    if(reachedPeakFlag && stopSway){
        deltaTimeSum = 0.0f;
        swayEnabled = false;
        stopSway = false;
        timeSumSign = 1.0f;
        return FVector(0,0,0);
    }



    //sin(time / x)
    float interpolatedValue = timeSumSign * std::sin(deltaTimeSum / fullMotionStretchSinWaveDenominator);
    FVector outvector(
        0.0f,
        interpolatedValue * heightY, //strecken des outputs
        interpolatedValue * heightZ
    );
   

    return outvector;
}


bool SwayInterpolator::reachedLoop(){
    bool copy = reachedPeakFlag;
    reachedPeakFlag = false; //auto reset flag
    return copy;
}