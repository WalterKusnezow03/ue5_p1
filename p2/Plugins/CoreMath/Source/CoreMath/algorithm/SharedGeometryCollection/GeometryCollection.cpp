#include "GeometryCollection.h"




void FGeometryCollection::Clear(){
    rawNodes.Empty();
    rawConvexHulls.Empty();
}

void FGeometryCollection::AddConvexHull(TArray<FVector> &array){
    rawConvexHulls.Add(array);
}

void FGeometryCollection::AddRawNodes(TArray<FVector> &array){
    if(array.Num() > 0){
        rawNodes.Append(array);
    }
}

void FGeometryCollection::AddOffsetRawNodes(FVector offsetIn){
    for (int i = 0; i < rawNodes.Num(); i++){
        FVector &current = rawNodes[i];
        current += offsetIn;
    }
}

TArray<FVector> &FGeometryCollection::GetRawNodes(){
    return rawNodes;
}



TArray<TArray<FVector>> &FGeometryCollection::GetRawConvexHulls(){
    return rawConvexHulls;
}


