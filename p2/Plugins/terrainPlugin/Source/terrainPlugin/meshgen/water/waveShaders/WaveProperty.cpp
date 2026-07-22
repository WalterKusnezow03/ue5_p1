#include "WaveProperty.h"

void WaveProperty::computeFrequency(){
    frequency = speed / waveLength;
}

void WaveProperty::UpdateShaderRunningTime(float time){
    shaderRunningTime = time;
}

void WaveProperty::SetWaveLength(float lambda){
    waveLength = std::abs(lambda);
    computeFrequency();
}

void WaveProperty::SetWaveSpeed(float speedIn){
    speed = std::abs(speedIn);
    computeFrequency();
}

void WaveProperty::SetAmplitude(float a){
    amplitude = std::abs(a);
}

float WaveProperty::GetAmplitude(){
    return amplitude;
}

float WaveProperty::GetFrequency(){
    return frequency;
}

float WaveProperty::GetSpeed(){
    return speed;
}

void WaveProperty::SetWaveSteepNess(float s){
    //wellen steilheit darf nur in range 0,1 sein.
    waveSteepNess = FMath::Clamp(s, 0.0f, 1.0f);
}


void WaveProperty::SetWaveDirection(const FVector2D &dir){
    waveDirection = dir.GetSafeNormal();
}

FVector2D WaveProperty::GetWaveDirection(){
    return waveDirection;
}


float WaveProperty::GetWaveLengthSave(){
    //Division durch 0 oder winzige Wellenlängen vermeiden.
    return FMath::Max(1.0f, waveLength);
}


float WaveProperty::GetWaveSteepNess(){
    return waveSteepNess;
}
