#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"

#include "WidgetProgressQuadCircular.generated.h"

/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.
/// tick still needed to listen for cursor position and animations!
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetProgressQuadCircular : public UWidgetSlateWrapperBase{
    GENERATED_BODY()

protected:
    //call super!
    //Custom Constructor override - is called internally from post properties override!
    virtual void ConstructWidget() override;

    float progressCopy = 1.0f;

public:

    void SetColorBackground(FLinearColor color);
    void SetColorForeground(FLinearColor color);

    ///@brief set progress of circle, between 0 and 1
    void SetProgress(float scalar);
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

    float TileSize();
    int TilesTotal();
    int TilesPerAxis();

    SlateMeshDataPolygon &PolygonBackground();
    SlateMeshDataPolygon &PolygonForeGround(int tileId);

    int setupResolution = 100;

    void CreateBackground();
    void CreateTiles();
    void AppendTile(FVector2D pivot, int tileId);

    void SetDefaultColors();

    float ClampProgress(float scalar);

    float debugTime = 0.0f;
};