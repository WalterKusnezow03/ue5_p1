#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"
#include "MaterialCompiler.h"

#include "CustomMaterialPlugin/Materials/Nodes/vertexShaderBase/WKVertexShaderBase.h"

#include "WKWaterShader.generated.h"


UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UWKWaterShader : public UWKVertexShaderBase
{
    GENERATED_BODY()

public:
    /// ---- Changable attributes of the water ----
 
    UPROPERTY(EditAnywhere, Category=UWKWaterShader)
    float Amplitude = 10.0f;

    UPROPERTY(EditAnywhere, Category=UWKWaterShader)
    float IntervallSlowDown = 1.0f; 

    UPROPERTY(EditAnywhere, Category=UWKWaterShader)
    float DistanceBetweenWaves = 500.0f; //every 1 meter

#if WITH_EDITOR

    //convert to layer codes
    int32 AmplitudeCode(FMaterialCompiler *Compiler){
        return MakeConstant(Compiler, Amplitude);
    }

    int32 Frequency(FMaterialCompiler *Compiler){
        //float frac = 100.0f / DistanceBetweenWaves;
        float f = 2.0f * M_PI / DistanceBetweenWaves; // 1 Welle pro DistanceBetweenWaves Meter
        return MakeConstant(Compiler, f);
    }



    int32 RunTime(FMaterialCompiler *Compiler){
        int32 TimeNow = GetTime(Compiler); //t,t
        return TimeNow;
    }
    int32 Speed(FMaterialCompiler *Compiler){
        return MakeConstant(Compiler, IntervallSlowDown);
    }

    virtual FString NodeName() const override {
        return TEXT("WKWaterShader");
    }

    virtual int32 Compile(FMaterialCompiler* Compiler, int32 OutputIndex) override
    {
        
        int32 x_frequency = Compiler->Mul(WorldPosX(Compiler), Frequency(Compiler));
        int32 y_frequency = Compiler->Mul(WorldPosY(Compiler), Frequency(Compiler));

        int32 time = RunTime(Compiler);
        int32 speed = Speed(Compiler);

        //shaderRunningTime * speed
        int32 scaledTime = Compiler->Mul(time, speed);

        int32 x = Compiler->Add(x_frequency, scaledTime);
        int32 y = Compiler->Add(y_frequency, scaledTime);

        int32 sinX = Compiler->Sine(x);
        int32 cosY = Compiler->Cosine(y);

        int32 waveSumAndOffset = Compiler->Add(sinX, cosY);
        int32 zResult = Compiler->Mul(waveSumAndOffset, AmplitudeCode(Compiler));

        //int32 MakeVector3D(FMaterialCompiler *compiler, int32 X, int32 Y, int32 Z)
        int32 result = MakeVector3D(
            Compiler, 
            MakeConstant(Compiler, 0.0f),
            MakeConstant(Compiler, 0.0f),
            zResult
        );
        return result;

        /*
        /// @brief apply vertex shader to the given vertex
        /// @param vertex vertex to move
        void AcustomWaterActor::applyShaderToVertex(FVector &vertex){
            FVector actorLocation = GetActorLocation();
            float distXAll = vertex.X + actorLocation.X;
            float distYAll = vertex.Y + actorLocation.Y;

            float frequency = 0.01f; // Wellenbreite
            float amplitude = 10.0f; // Wellenhöhe
            float speed = 1.0f; // Wellengeschwindigkeit //1.0;
            float wave = sin(distXAll * frequency + shaderRunningTime * speed) + 
                        cos(distYAll * frequency + shaderRunningTime * speed);

            vertex.Z = wave * amplitude;

        }
        
        
        
        
        
        */

    }
#endif
};