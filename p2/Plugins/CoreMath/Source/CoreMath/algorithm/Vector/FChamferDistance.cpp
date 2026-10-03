#include "FChamferDistance.h"

/*
float FChamferDistance::Loss(
    const TArray<FIntPoint> &Pset,
    const TArray<FIntPoint> &Qset
){
    float n = current.Num() + other.Num();
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

float FChamferDistance::LossFromPToQ(
    const TArray<FIntPoint> &Pset,
    const TArray<FIntPoint> &Qset
){
    //für alle punkte in P den kleinsten abstand in Q suchen
    float Loss = 0.0f;
    for (int i = 0; i < Pset.Num(); i++){
        Loss += Min(Qset, Pset[i]);
    }
    return Loss;
}

float FChamferDistance::Min(
    const TArray<FIntPoint> &Pset,
    const FIntPoint &other
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

float FChamferDistance::Dist2(const FIntPoint &a, const FIntPoint &b){
    float x = (b.X - a.X);
    float y = (b.Y - a.Y);

    // |a| = sqrt(a^2)
    return x * x + y * y;
}*/
