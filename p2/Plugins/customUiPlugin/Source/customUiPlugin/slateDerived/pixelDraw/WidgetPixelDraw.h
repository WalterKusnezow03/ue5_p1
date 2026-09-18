#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"

#include "WidgetPixelDraw.generated.h"

/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.
/// allows to draw individual pixels of the widget
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetPixelDraw : public UWidgetSlateWrapperBase{
    GENERATED_BODY()

protected:
    //call super!
    //Custom Constructor override - is called internally from post properties override!
    virtual void ConstructWidget() override;


public:
    void SetScale(float scale);

    void SetColorPixel(int x, int y, FLinearColor color);
    void SetColorPixel(int x, int y, FColor color);

    void Clear();

private:
    int width = 200;
    int height = 200;


    void CreatePixels();
    void CreatePixel(int x, int y);
    SlateMeshDataPolygon &FindData(int x, int y);
};