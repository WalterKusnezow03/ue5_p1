#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"
#include "customUiPlugin/slateDerived/progress/base/WidgetProgressBase.h"

#include "WidgetProgressBarBase.generated.h"

/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.
/// tick still needed to listen for cursor position and animations!
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetProgressBarBase : public UWidgetProgressBase{
    GENERATED_BODY()

protected:
    //call super!
    //Custom Constructor override - is called internally from post properties override!
    virtual void ConstructWidget() override;


public:

    void SetColorBackground(FLinearColor color);
    void SetColorForeground(FLinearColor color);

    ///@brief set progress of bar, between 0 and 1
    void SetProgress(float scalar);
    void ResetProgress();

private:
    int layerBackground = 0;
    int layerForeGround = 1;
    SlateMeshDataPolygon &PolygonBackground();
    SlateMeshDataPolygon &PolygonForeGround();

    void CreateBar();

    float ClampProgress(float scalar);

    float debugTime = 0.0f;
};