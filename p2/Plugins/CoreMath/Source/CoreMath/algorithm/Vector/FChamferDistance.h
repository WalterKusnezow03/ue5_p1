#pragma once

#include "CoreMinimal.h"

/// @brief will compute the champfer distance of 2 Point sets.
/// (The Similarity / Loss) of two unsorted point sets
template <typename T>
class COREMATH_API TChamferDistance{
public:

    float Similarity(
        const TArray<T> &Pset,
        const TArray<T> &Qset
    ){
        float loss = Loss(Pset, Qset);
        float similarity = FMath::Clamp(1.0f - loss, 0.0f, 1.0f);
        return similarity;
    }

    float Loss(
        const TArray<T> &Pset,
        const TArray<T> &Qset
    ){
        float n = Pset.Num() + Qset.Num();
        if(n <= 0.0f){
            return 0.0f;
        }

        //\(\text{CD}(P,Q)=\sum _{p\in P}\min _{q\in Q}\|{}p-q\|{}^{2}+\sum _{q\in Q}\min _{p\in P}\|{}q-p\|{}^{2}\)

        //bei unterschiedlichen set sizes notwendig
        //alle aus p zu q
        //alle aus q zu p

        float Loss = 0.0f;
        Loss += LossFromPToQ(Pset, Qset);
        Loss += LossFromPToQ(Qset, Pset);

        return Loss / n;
    }




protected:

    float LossFromPToQ(
        const TArray<T> &Pset,
        const TArray<T> &Qset
    ){
        //für alle punkte in P den kleinsten abstand in Q suchen
        float Loss = 0.0f;
        for (int i = 0; i < Pset.Num(); i++){
            Loss += Min(Qset, Pset[i]);
        }
        return Loss;
    }

    float Min(
        const TArray<T> &Pset,
        const T &other
    ){
        if(Pset.Num() <= 0){
            return 0.0;
        }
        float dist = FLT_MAX;
        for (int i = 0; i < Pset.Num(); i++){
            dist = std::min(Dist2(Pset[i], other), dist);
        }
        return dist;
    }

    virtual float Dist2(const T &a, const T &b) = 0;
};

/// @brief Chamfer-Distanz speziell für FIntPoint (2D Integer)
class COREMATH_API FIntPointChamferDistance : public TChamferDistance<FIntPoint> {
protected:
    float Dist2(const FIntPoint &a, const FIntPoint &b) override {
        float x = (b.X - a.X);
        float y = (b.Y - a.Y);
        return x * x + y * y;
    }
};

/// @brief Chamfer-Distanz für FVector (3D Float)
class COREMATH_API FVectorChamferDistance : public TChamferDistance<FVector> {
protected:
    float Dist2(const FVector &a, const FVector &b) override {
        FVector Diff = b - a;
        return Diff.SizeSquared(); // Nutzt die optimierte Unreal-Funktion für quadrierten Abstand
    }
};