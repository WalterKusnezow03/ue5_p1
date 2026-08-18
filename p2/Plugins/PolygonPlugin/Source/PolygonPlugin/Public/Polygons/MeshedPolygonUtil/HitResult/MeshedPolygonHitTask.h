#pragma once

#include "CoreMinimal.h"


class POLYGONPLUGIN_API FMeshedPolygonHitTask {

public:
    FMeshedPolygonHitTask();
    ~FMeshedPolygonHitTask();

    FMeshedPolygonHitTask(float maxT);

    FMeshedPolygonHitTask(const FVector2D &direction, float MaxT);

    void SetMaxT(float maxT);
    void SetDirection(const FVector2D &dir);

    /// @brief creates the maxT from the raw direction length!
    /// @param directionIn 
    void SetMaxTAndDirection(FVector2D directionIn);
    void SetMaxTAndDirection(const FVector2D &dir, float maxT);


    const FVector2D &GetDirection() const;

    void SetFarHit(bool flag);

    bool Update(float tIn, FVector2D result);
    void UpdateHitEdgeIndex(int index);

    FVector GetHitPoint3D();
    FVector2D GetHitPoint2D();
    FIntPoint GetHitIntPoint();
    float GetT();
    bool WasHit();

    int GetHitEdgeIndex();

    void AddIgnoredEdge(int index);
    bool IsIgnoredEdge(int index);
    bool IsIgnoredEdge(int index, int next);

    int NumHits();
    bool NumHitsIsEven();

    float GetMaxT(){
        return t;
    }

private:
    FVector2D direction;

    bool wasHit = false;
    FVector2D hitPoint;
    float t = 0.0f;
    int outHitEdgeIndex = -1;

    bool farHit = false;

    TArray<int> ignoredEdges;

    int numHitsUpdated = 0;
};