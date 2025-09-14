#pragma once

#include "CoreMinimal.h"

#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"
#include "CoreMath/algorithm/PolygonFit/polygon/MPolygon.h"

#include "GreedyFitWidget.generated.h"

UCLASS()
class P2_API UGreedyFitWidget : public UWidgetSlateWrapperBase{
    GENERATED_BODY()


public:
    virtual void ConstructWidget() override;

private:
    void AppendIntoLayer(MPolygon &polygon, int layer);
    void TestPolygonFit(TArray<MPolygon> &outPolygons);
};