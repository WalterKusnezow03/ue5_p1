#pragma once

#include "CoreMinimal.h"
#include "terrainPlugin/meshgen/generation/TerrainCreator/Road/InternalHelper/PolygonConstruction/RoadSection.h"

class TERRAINPLUGIN_API FRoadSectionList {


public:
    FRoadSectionList() {};
    ~FRoadSectionList() {};

    FRoadSectionList(const FRoadSectionList &other){
        if(this != &other){
            *this = other;
        }
    }

    FRoadSectionList &operator=(const FRoadSectionList &other){
        if(this != &other){
            sections = other.sections;
        }
        return *this;
    }

    void Add(FRoadSection &other){
        sections.Add(other);
    }

private:
    TArray<FRoadSection> sections;
};