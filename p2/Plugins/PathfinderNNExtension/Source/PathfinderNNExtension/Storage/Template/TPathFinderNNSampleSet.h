#pragma once

#include "CoreMinimal.h"
#include "DebugPlugin/DebugHelper.h"
#include "NNCommunicationPlugin/Storage/TNNSampleSetBase.h"

//template sample set for any "FMeshedPolygonTrajectoryLayeredInterface"
//todo: TSubClassOf must be ok / assert
template<typename T>
class PATHFINDERNNEXTENSION_API TPathFinderNNSampleSet : public TNNSampleSetBase<T>{
    // Compile-time check: T must inherit from FMeshedPolygonTrajectoryLayeredInterface
    static_assert(
        std::is_base_of_v<FMeshedPolygonTrajectoryLayeredInterface, T>, 
        "T must derive from FMeshedPolygonTrajectoryLayeredInterface"
    );

public:
    
    TPathFinderNNSampleSet() : TNNSampleSetBase<T>(){
        ExtractSampleType();
    }

    bool IsSampleTypeOf(EPolygonSampleType other){
        return other == polygonSampleTypeSaved;
    }

protected:
    EPolygonSampleType polygonSampleTypeSaved = EPolygonSampleType::none;
    void ExtractSampleType(){
        T empty;
        polygonSampleTypeSaved = empty.GetType();
    }
};