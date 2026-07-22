#pragma once

#include "CoreMath/animation/KeyFrames/KeyFrameAnimation.h"
#include "CoreMath/animation/timer/PayloadInterface/Payload.h"

class COREMATH_API PayloadKeyFrameAnimation : public KeyFrameAnimation{
public:
    /*PayloadKeyFrameAnimation(){

    }

    virtual ~PayloadKeyFrameAnimation(){

    }*/


    void AddPayload(Payload &load){
        executeOnFinish.Add(load);
    }

    bool PayloadExecuted(){
        return payloadExecuted;
    }

    virtual bool reachedLastFrameOfAnimation() override {
        if(PayloadExecuted()){
            return true;
        }
        return KeyFrameAnimation::reachedLastFrameOfAnimation();
    }

private:
    TArray<Payload> executeOnFinish;
    bool payloadExecuted = false;

protected:
    virtual void OnFinish() override {
        payloadExecuted = true;
        for (int i = 0; i < executeOnFinish.Num(); i++){
            Payload &current = executeOnFinish[i];
            current.Execute();
        }
    };

private:


};