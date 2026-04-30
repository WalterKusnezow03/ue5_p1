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

private:
    int maxElements = 1;
    int num = 0;
    TDoubleLinkedList<Trajectory> trajectories;

    void MoveToLocalTime(TArray<Trajectory> &outArray) const;

    void PopFront();
    void CopyList(const TrajectoryCollection &other);
};