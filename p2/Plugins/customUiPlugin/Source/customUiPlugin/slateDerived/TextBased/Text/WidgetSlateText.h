#pragma once

#include "CoreMinimal.h"

#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"
#include "WidgetSlateText.generated.h"

///@brief wont do anything except providing a simple box with a text.
///will setup design, font etc on its own. Default design.
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetSlateText : public UWidgetSlateWrapperBase{
    GENERATED_BODY()

public:
    void SetText(FString text);
    FString GetText();


protected:
    FVector2D setupScale();
    FVector2D MaxTextSize(FVector2D &bound);
    FVector2D MaxTextSizeDefault();

    virtual void ConstructWidget() override;
    void CreateTextBox();

    SlateMeshDataPolygon &PolygonAtTextLayer();
    int textLayer = 0;

};