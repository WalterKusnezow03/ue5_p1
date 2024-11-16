// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/KeyFrame.h"

KeyFrame::KeyFrame(FVector positionIn, float timeFromLastFrameIn)
{
    position = positionIn;
    setTime(timeFromLastFrameIn);
    grounded = false;
}

KeyFrame::KeyFrame(FVector positionIn, float timeFromLastFrameIn, bool groundedIn){
    position = positionIn;
    setTime(timeFromLastFrameIn);
    grounded = groundedIn;
}

KeyFrame::~KeyFrame()
{
}

void KeyFrame::setTime(float timeFromLastFrameIn){
    timeFromLastFrame = timeFromLastFrameIn;
}

FVector KeyFrame::readposition(){
    return position;
}

float KeyFrame::timeToFrame(){
    return timeFromLastFrame;
}

bool KeyFrame::mustBeGrounded(){
    return grounded;
}