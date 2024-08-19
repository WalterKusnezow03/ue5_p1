// Fill out your copyright notice in the Description page of Project Settings.


#include "timer.h"

timer::timer()
{
}

timer::~timer()
{
}

void timer::Begin(float time){
    timeLeft = time;
}

void timer::Tick(float deltaTime){
    timeLeft -= deltaTime;
}

bool timer::timesUp()
{
    return timeLeft < 0.05f;
}