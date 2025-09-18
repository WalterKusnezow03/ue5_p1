#pragma once

#include "CoreMinimal.h"

#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"
#include "WidgetImage.generated.h"

///@brief wont do anything except providing a simple box with a text.
///will setup design, font etc on its own. Default design.
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetImage : public UWidgetSlateWrapperBase{
    GENERATED_BODY()

public:
    /// @brief set image with auto resize widget
    /// @param image 
    void SetImage(UTexture2D *image);
    void SetImage(UTexture2D *image, FVector2D fixedRes);

    void RemoveImage();

    virtual void Tick(float deltatime) override;

protected:
    FVector2D validateResolution(FVector2D &other);

    virtual void ConstructWidget() override;
    void CreateImageBase();

    SlateMeshDataPolygon &PolygonAtBackgroundLayer();
    SlateMeshDataPolygon &PolygonAtImageLayer();
    int imageLayer = 0;

};