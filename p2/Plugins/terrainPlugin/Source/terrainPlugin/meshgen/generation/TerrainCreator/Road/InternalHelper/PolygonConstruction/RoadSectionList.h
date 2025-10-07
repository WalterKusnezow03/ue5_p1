#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/PolygonConstruction/RoadSection.h"

class TERRAINPLUGIN_API FRoadSectionList {


public:
    FRoadSectionList();
    ~FRoadSectionList();

    FRoadSectionList(const FRoadSectionList &other);

    FRoadSectionList &operator=(const FRoadSectionList &other);

    /// @brief sorts the road sections from 
    /// @param other
    void Add(FRoadSection &other);

    //ordered in realtion to previous section
    void AddOrdered(FRoadSection &other);

    //add get all ids here Tarray(ids)

    /// @brief get all road ids to build buffer from roads, for 
    /// copying road data from section definitions.
    /// @return 
    TArray<int> getRoadIds();

    ///@brief builds the polygon from the sections definitions automatically
    ///road ids found from data given.
    ///Do not execute this threaded, ptrs used internally. Do not modify array before finished
    void BuildPolygonAutoExtract(TArray<RoadData> &allRoads);
    TArray<FVector2D> &BuildedFromSections();

private:    
    /// @brief build polygon from ordered roads by getRoadIdsArray, from all roads extracted.
    TArray<FVector2D> BuildPolygonFromOrderedRoads(TArray<RoadData *> &roads);

    TArray<FRoadSection> sections;

    /// @brief builded from road sections
    TArray<FVector2D> buildedCombinedSpline;
};