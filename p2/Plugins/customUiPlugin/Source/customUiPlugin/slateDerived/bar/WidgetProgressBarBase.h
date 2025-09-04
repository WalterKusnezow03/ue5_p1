#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"

#include "WidgetProgressBarBase.generated.h"

/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.
/// tick still needed to listen for cursor position and animations!
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetProgressBarBase : public UWidgetSlateWrapperBase{
    GENERATED_BODY()

protected:
    //Custom Constructor override - is called internally from post properties override!
    virtual void ConstructWidget() override;


public:

    void SetColorBackground(FLinearColor color);
    void SetColorForeground(FLinearColor color);

    ///@brief set progress of bar, between 0 and 1
    void SetProgress(float num);
    void ResetProgress();

    //overriden but super call needed
    virtual void Tick(float deltatime) override;

    virtual bool dispatchClick() override;

private:
    int layerBackground = 0;
    int layerForeGround = 1;
    SlateMeshDataPolygon *PolygonBackground();
    SlateMeshDataPolygon *PolygonForeGround();

    void CreateBar();

    float ClampProgress(float num);

    float debugTime = 0.0f;
};