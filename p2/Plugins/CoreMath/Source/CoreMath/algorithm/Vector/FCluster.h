#pragma once

class COREMATH_API FCluster {

public:
    void Add(const FVector &pos);
    float Distance2(const FVector &pos);
    bool HasPositions();

    FVector Mean();

private:
    TArray<FVector> positions;
};