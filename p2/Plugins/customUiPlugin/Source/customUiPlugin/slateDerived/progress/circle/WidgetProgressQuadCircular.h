#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"
#include "customUiPlugin/slateDerived/progress/base/WidgetProgressBase.h"

#include "WidgetProgressQuadCircular.generated.h"

/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.
/// tick still needed to listen for cursor position and animations!
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetProgressQuadCircular : public UWidgetProgressBase{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category="WidgetData")
    float tileResolution;

    

protected:
    //call super!
    //Custom Constructor override - is called internally from post properties override!
    virtual void ConstructWidget() override;


public:

    void SetColorsFromProperty();
    void SetColorBackground(FLinearColor color);
    void SetColorForeground(FLinearColor color);

    ///@brief set progress of circle, between 0 and 1
    virtual void SetProgress(float scalar) override;
    void ResetProgress();

    void EnableTileRangeByScalar(
        float scalarStart,
        float scalarEnd,
        bool enableRender
    );

private:

    void EnableTileRangeById(
        int startId,
        int endId,
        bool enableRender
    );



    int layerBackground = 0;
    int VerifyTileIdToPolygonId(int tileId);


    int TilesTotal();
    int TilesPerAxisX();
    int TilesPerAxisY();
    int TilesPerAxis(double axis, double sizetile);

    SlateMeshDataPolygon &PolygonBackground();
    SlateMeshDataPolygon &PolygonForeGround(int tileId);

    

    void CreateBackground();
    void CreateTiles();
    void AppendTile(FVector2D pivot, int tileId);

  
};