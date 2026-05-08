#pragma once

#include "StoragePlugin/Storage/BaseInterface/StorageInterface.h"

class FMeshedPolygonTrajectoryLayered;

class PATHFINDERNNEXTENSION_API FPathFinderNNSampleStorageInterface : public StorageInterface {

public:
    FPathFinderNNSampleStorageInterface();
    virtual ~FPathFinderNNSampleStorageInterface() override;

    void Save(TArray<FMeshedPolygonTrajectoryLayered> &data);
    bool Load(TArray<FMeshedPolygonTrajectoryLayered> &dataOut);

private:
    FString partnameFallback = "binary1";
    FString makePath(FString part);
};