#include "CpuWaterVertexShaderTrochoidal.h"

void CpuWaterVertexShaderTrochoidal::SetWaveSteepNess(float s){
    waveProperty.SetWaveSteepNess(s);
}

void CpuWaterVertexShaderTrochoidal::SetWaveDirection(FVector2D &dir){
    waveProperty.SetWaveDirection(dir);
}

//simplified
void CpuWaterVertexShaderTrochoidal::applyShaderToVertex(
    FVector &vertex, 
    const FVector &actorLocation,
    WaveProperty &wavePropertyIn
){
    float waveLength = wavePropertyIn.GetWaveLengthSave(); //lambda
    FVector2D waveDir = wavePropertyIn.GetWaveDirection().GetSafeNormal();

    //float a = vertex.X;
    //float a1 = vertex.Y;
    //float b = vertex.Z;

    const float k = 2.0f * PI / waveLength;
    const float g = 981.0f;
    const float c = FMath::Sqrt(g / k); //c^2 = g / k

    //const float kact = k * (a + c * shaderRunningTime); //k * (a+ct)
    
    //Globale Weltposition für nahtlose Phase über Mesh-Grenzen hinweg
    FVector2D worldPos = WorldPos2D(vertex, actorLocation);

    //Phasenwinkel f (Projektion der Position auf die Wellenrichtung)
    // f = k * ( (Pos . Dir) - c * t )
    // für logisches a, wo ist der vertex in der welt
    // statt manuell a_x und a_y zu berechnen ist das skalar produkt hier hinreichend
    const float dotProduct = FVector2D::DotProduct(worldPos, waveDir); 
    //welle bewegt sich mit der zeit t.
    //- für korrekte richtung nach rechts, sonst +, nach links
    const float f = k * (dotProduct - c * shaderRunningTime * wavePropertyIn.GetSpeed());

    float sinF, cosF;
    FMath::SinCos(&sinF, &cosF, f);

    //aus wikipedia aber sehr instabil:
    //b = 0 at surface level
    //e^(kb) = 1
    //float OneOverK = 1.0f / k; //oder amplitude
    //OneOverK *= wavePropertyIn.GetAmplitude();

    //besser und stabiler mit konstanten für amplitude und stepness (1/k sehr instabil)
    float OneOverK = wavePropertyIn.GetAmplitude() * wavePropertyIn.GetWaveSteepNess();

    //float offX = a + OneOverK * sinF; //gilt für wave dir dot product = 1
    //float offY = a1 + OneOverK * sinF;
    float offsetHorizontal = OneOverK * sinF;

    vertex.X -= waveDir.X * offsetHorizontal;
    vertex.Y -= waveDir.Y * offsetHorizontal;
    vertex.Z += wavePropertyIn.GetAmplitude() * cosF;

    /*float crestFactor = sinF; //Ist 1.0 am absoluten Kamm der Welle / brechpunkt
    if (crestFactor > 0.8f && waveSteepNess > 0.6f) {
        //steile welle
    }*/
}










