#pragma once

#include "CoreMinimal.h"
#include "BlockProperty.h"

#include "BuildingColorMapData.generated.h"

/// @brief stores color properties for building blocks
/// searchable by color and color offset
USTRUCT(BlueprintType)
struct TERRAINBUILDINGPLUGIN_API FBuildingColorMapData {
    GENERATED_BODY()


public:
    FBuildingColorMapData(){}

    //per color save properties of block
    UPROPERTY(EditAnywhere, Category="ColorMap")
    TMap<FColor, FBlockProperty> propertyMap;

    bool FindProperty(const FColor &color, FBlockProperty &outProperty) const;
    bool FindProperty(const FColor &color, FBlockProperty &outProperty, float colorOffsetAllowed) const;

    bool ColorMapIsValid();

protected:
    bool UpdateFound(const FColor &color, const FColor &other, float &maxDistance) const;
    float DistSquared(const FColor &color, const FColor &other) const;
    FVector ToVector(const FColor &color) const;
};
