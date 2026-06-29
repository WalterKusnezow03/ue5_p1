#pragma once

#include "CoreMinimal.h"
#include "Trajectory.h"


class PATHFINDERNNEXTENSION_API TrajectoryCollection {

public:
    TrajectoryCollection();
    ~TrajectoryCollection();
    TrajectoryCollection(const TrajectoryCollection &other);
    TrajectoryCollection &operator=(const TrajectoryCollection &other);

    void Setup(int maxSize);
    void Add(FVector &pos);
    int Num() const;
    int NumMax() const;

    TArray<Trajectory> ToArray() const;
    TArray<Trajectory> ToArrayLocalSpace(const FVector &worldLocation) const;

    TArray<Trajectory> ToArrayNormalizedTime() const;

    static bool EndDir(const TArray<Trajectory> &array, FVector2D &outDir);
    static bool GlobalDir(const TArray<Trajectory> &array, FVector2D &outDir);
    

private:
    int maxElements = 1;
    int num = 0;
    TDoubleLinkedList<Trajectory> trajectories;

    void MoveToLocalTime(TArray<Trajectory> &outArray) const;
    void NormalizeTime(TArray<Trajectory> &data) const;

    void PopFront();
    void CopyList(const TrajectoryCollection &other);


    static FVector Dir2D(const Trajectory &a, const Trajectory &b);
};