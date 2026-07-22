#pragma once

#include "CoreMinimal.h"
#include "CpuWaterVertexShaderBase.h"

//vertex shader base class (derived classes will change the shading function)
//for the procedural mesh actor.
//the performance is fairly stable with a not high vertex count
//and low render distance (up to 50  meters)
class TERRAINPLUGIN_API CpuWaterVertexShaderSimple : public CpuWaterVertexShaderBase {

public:

    

    virtual void applyShaderToVertex(
        FVector &vertex, 
        const FVector &actorLocation,
        WaveProperty &wavePropertyIn
    ) override {
        //to be overriden

        float distXAll = vertex.X + actorLocation.X;
        float distYAll = vertex.Y + actorLocation.Y;

        float frequency = wavePropertyIn.GetFrequency();
        float speed = wavePropertyIn.GetSpeed();

        float wave = sin(distXAll * frequency + shaderRunningTime * speed) + 
                    cos(distYAll * frequency + shaderRunningTime * speed);

        vertex.Z = wave * wavePropertyIn.GetAmplitude();


    }

    


protected:


};