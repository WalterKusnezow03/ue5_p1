#pragma once




/*
UV-Koordinaten
Compiler->TextureCoordinate(UVIndex, bFractional)
Gibt dir die UVs vom Mesh (pro Vertex interpoliert). UVIndex = 0 ist Standard.

World Position
Compiler->WorldPosition(EWorldPositionIncludedOffsets WorldPositionIncludedOffsets)
Liefert die Weltposition (verschiedene Varianten: absolut, ohne Offset, etc.).
Object Position / Vertex Position


Compiler->ObjectWorldPosition() oder Compiler->VertexColor()
Zugriff auf Vertex-Farbe oder Objektposition.
Vertex Normal / Tangent


Compiler->VertexNormal(), Compiler->VertexTangent()
Zugriff auf Mesh-Normalen/Tangenten.


Time
Compiler->GameTime(bPeriodic, bRealTime)
Zugriff auf Material-Zeit (für Animationen).


Camera Vector
Compiler->CameraVector()
Normalisierter Blickvektor von Kamera zum Pixel.


ScreenPosition
Compiler->ScreenPosition(...)
Position in Screenspace, für PostProcess/Screen-Effekte.

*/


#pragma once

#include "CoreMinimal.h"
#include "Materials/MaterialExpression.h"
#include "MaterialCompiler.h"
#include "CustomMaterialPlugin/Materials/Nodes/NodeBase.h"

#include "WKVertexShaderBase.generated.h"


UCLASS(collapsecategories, hidecategories=Object)
class CUSTOMMATERIALPLUGIN_API UWKVertexShaderBase : public UNodeBase
{
    GENERATED_BODY()

public:

#if WITH_EDITOR

    virtual FString NodeName() const override {
        return TEXT("WKVertexShaderBase");
    }

    //convert constants
    int32 MakeConstant(FMaterialCompiler *compiler, float num){
        return compiler->Constant(num);
    }

    // UV-Koordinaten
    int32 GetUV(
        FMaterialCompiler* Compiler, 
        int32 UVIndex = 0, 
        bool bFractional = false, 
        bool bUnMirror = false //idk what that is for
    )
    {
        return Compiler->TextureCoordinate(UVIndex, bFractional, bUnMirror);
    }

    // Weltposition
    int32 GetWorldPosition(
        FMaterialCompiler* Compiler, 
        EWorldPositionIncludedOffsets WorldPositionMode = WPT_Default
    )
    {
        return Compiler->WorldPosition(WorldPositionMode);
    }


    //make vertex
    int32 MakeVector3D(FMaterialCompiler *Compiler, int32 X, int32 Y, int32 Z){ //as layers.
        int32 XY = Compiler->AppendVector(X, Y); // 2D Vector: (x, y)
        int32 XYZ = Compiler->AppendVector(XY,Z); // 3D Vector: (x, y, z)
        return XYZ;
    }



    int32 WorldPosX(FMaterialCompiler *Compiler){
        int32 worldPos = GetWorldPosition(Compiler);
        int32 x = Compiler->ComponentMask(worldPos, true, false, false, false); // x,y,z,h
        return x;
    }

    int32 WorldPosY(FMaterialCompiler *Compiler){
        int32 worldPos = GetWorldPosition(Compiler);
        int32 y = Compiler->ComponentMask(worldPos, false, true, false, false); // x,y,z,h
        return y;
    }

    int32 WorldPosZ(FMaterialCompiler *Compiler){
        int32 worldPos = GetWorldPosition(Compiler);
        int32 z = Compiler->ComponentMask(worldPos, false, false, true, false); // x,y,z,h
        return z;
    }

    int32 QuadraticHorizontalDistanceFromOrigin(FMaterialCompiler *Compiler){
        int32 worldPos = GetWorldPosition(Compiler);
        int32 x = WorldPosX(Compiler);
        int32 y = WorldPosY(Compiler);

        int32 x2 = Compiler->Mul(x, x);
        int32 y2 = Compiler->Mul(y, y);

        int32 sum = Compiler->Add(x2, y2);
        int32 distance = Compiler->SquareRoot(sum);

        return distance;
    }

    // Objektposition
    int32 GetObjectPosition(FMaterialCompiler* Compiler)
    {
        return Compiler->ObjectWorldPosition();
    }

    // Vertex-Farbe
    int32 GetVertexColor(FMaterialCompiler* Compiler)
    {
        return Compiler->VertexColor();
    }

    // Vertex-Normal
    int32 GetVertexNormal(FMaterialCompiler* Compiler)
    {
        return Compiler->VertexNormal();
    }

    // Vertex-Tangent
    int32 GetVertexTangent(FMaterialCompiler* Compiler)
    {
        return Compiler->VertexTangent();
    }

    // Spielzeit
    int32 GetTime(FMaterialCompiler* Compiler, bool bPeriodic = false, bool bRealTime = true)
    {
        return Compiler->GameTime(bPeriodic, bRealTime);
    }

    // Kamerarichtung
    int32 GetCameraVector(FMaterialCompiler* Compiler)
    {
        return Compiler->CameraVector();
    }

    int32 GetScreenPositionAsPixel(FMaterialCompiler* Compiler)
    {
        return Compiler->GetPixelPosition(); // Liefert 0..1 UVs über den Screen
    }


#endif
};