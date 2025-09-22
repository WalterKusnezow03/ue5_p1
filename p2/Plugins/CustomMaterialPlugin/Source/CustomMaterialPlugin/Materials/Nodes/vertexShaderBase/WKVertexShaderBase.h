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

//#include "Materials/MaterialExpressionLocalPosition.h"

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
    int32 MakeConstant(FMaterialCompiler *compiler, float num);

    // UV-Koordinaten
    int32 GetUV(
        FMaterialCompiler *Compiler,
        int32 UVIndex = 0,
        bool bFractional = false,
        bool bUnMirror = false // idk what that is for
    );

    //World Vertex Location
    int32 GetWorldPosition(
        FMaterialCompiler *Compiler,
        EWorldPositionIncludedOffsets WorldPositionMode = WPT_Default
    );

    //Local Vertex Location, does not include rotation or scaling right now!
    int32 GetLocalPosition(FMaterialCompiler *Compiler);

    //make vertex
    int32 MakeVector2D(FMaterialCompiler *Compiler, int32 X, int32 Y);
    int32 MakeVector3D(FMaterialCompiler *Compiler, int32 X, int32 Y, int32 Z);

    int32 WorldPosX(FMaterialCompiler *Compiler);
    int32 WorldPosY(FMaterialCompiler *Compiler);
    int32 WorldPosZ(FMaterialCompiler *Compiler);

    int32 QuadraticHorizontalDistanceFromOrigin(FMaterialCompiler *Compiler);

    // Objektposition
    int32 GetObjectPosition(FMaterialCompiler *Compiler);

    // Vertex-Farbe
    int32 GetVertexColor(FMaterialCompiler *Compiler);

    // Vertex-Normal
    int32 GetVertexNormal(FMaterialCompiler *Compiler);

    // Vertex-Tangent
    int32 GetVertexTangent(FMaterialCompiler *Compiler);

    //RunTime
    int32 GetTime(FMaterialCompiler *Compiler, bool bPeriodic = false, bool bRealTime = true);
    
    //scaled time
    int32 ScaledTime(FMaterialCompiler *Compiler, float scalar);

    // Kamerarichtung
    int32 GetCameraVector(FMaterialCompiler *Compiler);
    int32 GetScreenPositionAsPixel(FMaterialCompiler *Compiler);

#endif
};