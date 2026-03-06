#include "BuildingColorMapData.h"


bool FBuildingColorMapData::ColorMapIsValid(){
    return propertyMap.Num() > 0;
}

bool FBuildingColorMapData::FindProperty(const FColor &color, FBlockProperty &outProperty)const{
    if (const FBlockProperty* property = propertyMap.Find(color)){
        outProperty = *property;
        return true;
    }
    return false;
}

bool FBuildingColorMapData::FindProperty(
    const FColor &color, 
    FBlockProperty &outProperty, 
    float colorOffsetAllowed
)const{
    FBlockProperty closestPair;
    float distance = colorOffsetAllowed * colorOffsetAllowed;
    bool anyFound = false;

    for (const TPair<FColor, FBlockProperty>& Pair : propertyMap){
        const FColor &key = Pair.Key;
        const FBlockProperty &value = Pair.Value;
        if(UpdateFound(key, color, distance)){
            closestPair = value;
            anyFound = true;
        }
    }
    if(anyFound){
        outProperty = closestPair;
    }
    return anyFound;
}

bool FBuildingColorMapData::UpdateFound(const FColor &color, const FColor &other, float &maxDistance)const{
    float dist = DistSquared(color, other);
    if(dist < maxDistance){
        maxDistance = dist;
        return true;
    }
    return false;
}

float FBuildingColorMapData::DistSquared(const FColor &color, const FColor &other)const{
    return FVector::DistSquared(ToVector(color), ToVector(other));
}


FVector FBuildingColorMapData::ToVector(const FColor &color)const{
    return FVector(color.R, color.G, color.A);
}