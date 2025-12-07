#pragma once

#include "CoreMinimal.h"

/// @brief saves the index of sift vectors which are matched, and their screen coordinates
class COMPUTERVISIONPLUGIN_API PotentialMatch {
public:
    PotentialMatch();
    ~PotentialMatch();

    PotentialMatch(int iIn, int jIn, float distanceIn);
    PotentialMatch(
        int iIn,
        int jIn,
        float distanceIn,
        FVector2D &postionA,
        FVector2D &postionB
    );

    PotentialMatch(const PotentialMatch &other);
    PotentialMatch &operator=(const PotentialMatch &other);

    float Distance() const;

    void Setup(int iIn, int jIn, float distanceIn);
    void Setup(
        int iIn, 
        int jIn, 
        float distanceIn, 
        FVector2D &postionA, 
        FVector2D &postionB
    );

    bool IndexIisSame(PotentialMatch &other){
        return i == other.i;
    }

    void IndexPair(int &iOut, int &jOut);

    FVector2D &GetPositionFirst();
    FVector2D &GetPositionSecond();


private:
    float distance = 0;
    int i = 0;
    int j = 0;
    FVector2D positionI;
    FVector2D positionJ;
};