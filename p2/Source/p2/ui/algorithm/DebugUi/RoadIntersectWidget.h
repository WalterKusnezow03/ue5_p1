#pragma once

#include "CoreMinimal.h"

#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"

#include "RoadIntersectWidget.generated.h"

UCLASS()
class P2_API URoadIntersectWidget : public UWidgetSlateWrapperBase{
    GENERATED_BODY()


public:
    virtual void ConstructWidget() override;

private:
    MMatrix2D MoveAndScaleToPivot0(
        TArray<std::pair<FVector2D, FVector2D>> &edges,
        FVector2D &scaleDesired
    );

    void AppendQuadFromEdge(FVector2D &a, FVector2D &b, SlateMeshData &data);
};