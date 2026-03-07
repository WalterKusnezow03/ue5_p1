#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"

#include "WidgetProgressBase.generated.h"

/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.
/// tick still needed to listen for cursor position and animations!
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetProgressBase : public UWidgetSlateWrapperBase{
    GENERATED_BODY()

protected:
    
    UPROPERTY(EditAnywhere, Category="WidgetData")
    float progressScalar = 0.6f;

    UPROPERTY(EditAnywhere, Category="WidgetData")
    FColor colorBackground;

    UPROPERTY(EditAnywhere, Category="WidgetData")
    FColor colorForeground;

public:


    ///@brief set progress of bar, between 0 and 1
    virtual void SetProgress(float scalar);
    void ResetProgress();


protected:
    float ClampProgress(float scalar);

    
};