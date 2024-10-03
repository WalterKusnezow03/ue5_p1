// Fill out your copyright notice in the Description page of Project Settings.


#include "abstractKinematicFunctions.h"

abstractKinematicFunctions::abstractKinematicFunctions()
{
}

abstractKinematicFunctions::~abstractKinematicFunctions()
{
}



/// @brief leg lift function
/// @param t must be between -pi and pi, must be radian
/// @return function value etha(?)
float abstractKinematicFunctions::legLift(float t){

    float result = 0.0f;
    if (t <= 0)
    {
        result = std::sin(t) * (c6 + c7); //scales down sin to not move leg to high
    }
    else
    {
        result = std::sin(t) * (c8 + c9);
    }

    return result;
}

/// @brief leg swing param, t in radian, return in radian
/// @param t t radian between [-pi, pi), must be clamped outside the function!
/// @return radian leg swing pitch around y 
float abstractKinematicFunctions::legSwingPitch(float t){

    //float velocity = 1; // 1 vorwärts, -1 rückwärts percent fraction

    float offset = MMatrix::degToRadian(30);
    //return std::sin(t) * 0.3f;
    return std::sin(t + offset) * 0.3f; // MUST BE COS TO SWING FORWARD AND BACKWARD, 0 IS UP POINT
    // cos
}

float abstractKinematicFunctions::pitchlegSwing(float t){

    float gamma = 0.0f;
    if(ct0 <= t && t <= ct1){
        gamma = (std::cos(((t - ct0) / (ct1 - ct0)) * M_PI));
    }

    float denominator = (2 * M_PI - ct1 + ct0);
    if (ct1 <= t && t < M_PI)
    {
        gamma = ((2*(t - ct1) / denominator)) - 1;
    }
    if(-1* M_PI <= t && t < ct0){
        gamma = ((2*(t + 2 * M_PI - ct1) / denominator)) - 1;
    }

    float pitchlegResult = 0.0f; //waaat

    if(velocity >= 0){
        pitchlegResult = gamma * c10 * velocity;
    }
    else
    {
        pitchlegResult = gamma * c11 * velocity;
    }

    //woher kommt vx und vy??? (velocity??)
    //pitchlegResult noch zutun.
    return pitchlegResult;
}