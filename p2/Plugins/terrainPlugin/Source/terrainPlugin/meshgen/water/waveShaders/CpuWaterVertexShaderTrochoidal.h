#pragma once

#include "CoreMinimal.h"
#include "CpuWaterVertexShaderBase.h"

//vertex shader base class (derived classes will change the shading function)
//for the procedural mesh actor.
//the performance is fairly stable with a not high vertex count
//and low render distance (up to 50  meters)
class TERRAINPLUGIN_API CpuWaterVertexShaderTrochoidal : public CpuWaterVertexShaderBase {


public:
    void SetWaveSteepNess(float s);
    void SetWaveDirection(FVector2D &dir);

    using CpuWaterVertexShaderBase::applyShaderToVertex;

    virtual void applyShaderToVertex(
        FVector &vertex,
        const FVector &actorLocation,
        WaveProperty &wavePropertyIn
    ) override;

    

protected:


};