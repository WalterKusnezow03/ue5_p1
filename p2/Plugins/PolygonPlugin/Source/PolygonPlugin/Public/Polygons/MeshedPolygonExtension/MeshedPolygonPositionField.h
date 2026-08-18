#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

//adds the position grid to the meshed polygon
class POLYGONPLUGIN_API FMeshedPolygonPositionField : public FMeshedPolygon {

protected:
    TArray<TArray<FVector>> positionGrid;

    virtual void GenerateGrid() override;
    virtual bool GridValid() const override;

    

    
    bool PositionAtFlag(int x, int y, FVector &outPos);

    bool PositionAt(int x, int y, FVector &outPos);
    bool PositionAt(const std::pair<int, int> &pair, FVector &outPos);

    //is in world space (with pivot offset)
    FVector GetPositionAt(const std::pair<int, int> &pair);

    //debug
    void AppendAt(int i, int j, MeshData &data, bool removeOffset2D);
    TArray<FVector> GetQuadOrTriangleAt(int i, int j);

    //created with respect to world pivot.
    void MakePositionGrid();

    void SetPosition(int x, int y, const FVector &pos);

    //updates the world height of the given world coordinate
    void UpdateWorldHeightFor(FVector &asWorldPos);

public:
    TArray<TArray<FVector>> &GetPositionGrid();
    const TArray<TArray<FVector>> &GetPositionGrid() const;

    //init from rasterized curve, to raster of mod "widthOfInsideStep"
    //forces a min size of one if bounds to small for pixel step "widthOfInsideStep"
    //virtual void InitForceSizeMin(TArray<FVector> &polygon, float widthOfInsideStep) override;

    void AppendMeshedSurface(MeshData &data);

    //remove offset for local mesh
    void AppendMeshedSurface(MeshData &data, bool removeOffset2D);

    FVector BottomLeft() override;
    FVector TopRight() override;

protected:
    virtual bool InitAsSinglePixel(TArray<FVector> &polygon, float widthOfInsideStep) override;
};