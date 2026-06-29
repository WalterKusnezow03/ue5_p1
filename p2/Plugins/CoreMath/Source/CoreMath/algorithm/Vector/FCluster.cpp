#include "FCluster.h"

void FCluster::Add(const FVector &pos){
    positions.Add(pos);
}

float FCluster::Distance2(const FVector &pos){
    if(HasPositions()){
        return FVector::DistSquared(pos, positions[0]);
    }
    return 0.0f;
}

FVector FCluster::Mean(){
    FVector sum;
    if(HasPositions()){
        float num = positions.Num();
        for (int i = 0; i < positions.Num(); i++){
            sum += positions[i];
        }
        return sum / num;
    }
    return sum;
}

bool FCluster::HasPositions(){
    return positions.Num() > 0;
}
