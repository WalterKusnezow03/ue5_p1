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
    virtual void OnHover() override;


    FVector2D setupScale();
    FVector2D MaxTextSize(FVector2D &bound);
    FVector2D MaxTextSizeDefault();

    virtual void ConstructWidget() override;
    void CreateTextBox();

    
    int textLayer = 0;

    //virtual void SetResolution(FVector2D scale) override;
    SlateMeshDataPolygon &PolygonAtTextLayer();

};