#pragma once 

#include "CoreMinimal.h"

/// @brief not tested at all.
struct CUSTOMUIPLUGIN_API FClipEdge {

    FClipEdge() {};
    ~FClipEdge() {};

    FClipEdge(const FClipEdge &other);
    FClipEdge &operator=(const FClipEdge &other);

    FClipEdge(const FVector2D &start, const FVector2D &dir);

    FVector2D Local(const FVector2D &other);

    bool RightOffLocal(const FVector2D &other);
    bool RightOffWorld(const FVector2D &other);

    bool ClipEndWorld(const FVector2D &a, FVector2D &b);

    FVector2D start;
    FVector2D end;
    FVector2D dirSized;
    FVector2D normal;

    FVector Make3D(const FVector2D &other);
};