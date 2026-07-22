#pragma once

#include "CoreMinimal.h"

class TERRAINPLUGIN_API WaveProperty {

public:
    void UpdateShaderRunningTime(float time);
    void SetWaveLength(float lambda);
    void SetWaveSpeed(float speedIn);
    void SetAmplitude(float a);
    void SetWaveSteepNess(float s);
    void SetWaveDirection(const FVector2D &dir);

    float GetAmplitude();
    float GetFrequency();
    float GetSpeed();
    FVector2D GetWaveDirection();
    float GetWaveSteepNess();

    float GetWaveLengthSave();

protected:
    float shaderRunningTime = 0.0f;
    FVector2D waveDirection;

    //y = c / f
    //c = y * f
    //f = c / y


    //f = c / y
    //f: frequency
    //y: wave length
    //c: speed

    void computeFrequency();

    float waveLength = 1.0f;
    float frequency = 0.01f; // Wellenbreite
    float amplitude = 10.0f; // Wellenhöhe
    float speed = 1.0f; // Wellengeschwindigkeit //1.0;
    float waveSteepNess = 0.5f;
};