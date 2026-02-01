#pragma once

#include "CoreMinimal.h"


class P2_API FMiniMapMarkerTransform {

public:
    FMiniMapMarkerTransform();
    ~FMiniMapMarkerTransform();

    FMiniMapMarkerTransform(const FMiniMapMarkerTransform &other);
    FMiniMapMarkerTransform &operator=(const FMiniMapMarkerTransform &other);

    FMiniMapMarkerTransform(const FVector2D &posIn, float angleIn);

    void SetLocation(const FVector2D &posIn);
    void SetAngle(const float angle);

    FVector2D &GetPositionRef();

    FVector2D Location() const;
    float Angle() const;

protected:
    float angleForItem = 0.0f;
    FVector2D locationForItem;

    //scale might be added.


};