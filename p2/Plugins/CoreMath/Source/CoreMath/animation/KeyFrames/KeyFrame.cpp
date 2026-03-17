// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyFrame.h"

KeyFrame::KeyFrame(FVector positionIn, float timeToFrameIn)
{
    position = positionIn;
    setTimeToFrame(timeToFrameIn);
    grounded = false;
}

KeyFrame::KeyFrame(FVector positionIn, float timeToFrameIn, bool groundedIn){
    position = positionIn;
    setTimeToFrame(timeToFrameIn);
    grounded = groundedIn;
}

KeyFrame::~KeyFrame()
{
}

void KeyFrame::setTimeToFrame(float timeToFrameIn){
    timeToFrame = std::abs(timeToFrameIn);
}

void KeyFrame::ScaleTimeToFrame(float scalar){
    timeToFrame *= scalar;
}

FVector KeyFrame::readposition(){
    return position;
}

float KeyFrame::readTimeToFrame(){
    return timeToFrame;
}

bool KeyFrame::mustBeGrounded(){
    return grounded;
}


float KeyFrame::distanceTo(const KeyFrame &other){
    return FVector::Dist(other.position, position);
}

bool KeyFrame::IsSame(const KeyFrame &other){
    return distanceTo(other) <= 1.0f;
}

FString KeyFrame::ToString(){
    FString outInfo = 
    FString::Printf(
        TEXT(
            "(x%.2f, y%.2f, z%.2f)(t:%.2f)"
        ),
        position.X,
        position.Y,
        position.Z,
        timeToFrame
    );

    return outInfo;
}