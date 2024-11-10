// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/animation/KeyFrame.h"

KeyFrame::KeyFrame(FVector positionIn, float timeFromLastFrameIn)
{
    position = positionIn;
    timeFromLastFrame = timeFromLastFrameIn;
    grounded = false;
}

KeyFrame::KeyFrame(FVector positionIn, float timeFromLastFrameIn, bool groundedIn){
    position = positionIn;
    timeFromLastFrame = timeFromLastFrameIn;
    grounded = groundedIn;
}

KeyFrame::~KeyFrame()
{
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