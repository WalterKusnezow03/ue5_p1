// Copyright Walter Kusnezow All Rights Reserved.


#pragma once

#include "CoreMinimal.h"

class ANYMESHWIDGETPLUGIN_API FAnyMeshWidgetRayIntersectResult {

public:
    FAnyMeshWidgetRayIntersectResult();
    FAnyMeshWidgetRayIntersectResult(const FAnyMeshWidgetRayIntersectResult &other);
    FAnyMeshWidgetRayIntersectResult &operator=(const FAnyMeshWidgetRayIntersectResult &other);

    ~FAnyMeshWidgetRayIntersectResult();

    bool IsResultValid();

    void SetResult(FVector2D &screenPos);

    const FVector2D &constScreenPositionReference();

private:
    bool bResultIsValid = false;
    FVector2D screenPosition;
};