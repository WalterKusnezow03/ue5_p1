// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer.h"



Timer::Timer(){

}

/// @brief starts the timer at a given time
/// @param time time will be absolute
/// @param resetsItselfIn tells if the timer resets itsself when the timesUpFunction returns true
Timer::Timer(float time, bool resetsItselfIn)
{
    Begin(time, resetsItself);
}

Timer::~Timer()
{
}


/// @brief starts the timer at a given time
/// @param time time will be absolute
/// @param resetsItselfIn tells if the timer resets itsself when the timesUpFunction returns true
void Timer::Begin(float time, bool resetsItselfIn){
    time = std::abs(time);
    initialTime = time;
    timeLeft = time;
    resetsItself = resetsItselfIn;
}

/// @brief will begin the timer, one time timer!
/// @param time 
void Timer::Begin(float time){
    Begin(time, false);
}

void Timer::Reset(){
    timeLeft = initialTime;
}

void Timer::Tick(float deltaTime){
    timeLeft -= deltaTime;
}

bool Timer::TickWithTimesUpReset(float deltatime){
    Tick(deltatime);
    return timesUp();
}

bool Timer::timesUp()
{
    bool timeIsUp = timeLeft < 0.05f;
    if(timeIsUp && resetsItself){
        timeLeft = initialTime;
    }
    return timeIsUp;
}

//between 0 and 1, reached or not
float Timer::scalar(){
    float reached = initialTime - timeLeft; 
    float denominator = std::max(initialTime, 0.1f);
    return reached / denominator;
}

//0 if reached. 1 if at time 0
float Timer::InvertedScalar(){
    float _scalar = 1.0f - scalar();
    return std::max(_scalar, 0.0f);
}