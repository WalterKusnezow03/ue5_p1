#pragma once

#include "CoreMinimal.h"
#include "DebugPlugin/DebugHelper.h"
#include "NNCommunicationPlugin/Storage/TNNSampleSetMultiPart.h"

//template sample set for any "FMeshedPolygonTrajectoryLayeredInterface"
//todo: TSubClassOf must be ok / assert
template<typename T>
class PATHFINDERNNEXTENSION_API TPathFinderNNSampleSetMultiPart : public TNNSampleSetBaseMultiPart<T>{
    // Compile-time check: T must inherit from FMeshedPolygonTrajectoryLayeredInterface
    static_assert(
        std::is_base_of_v<FMeshedPolygonTrajectoryLayeredInterface, T>, 
        "T must derive from FMeshedPolygonTrajectoryLayeredInterface"
    );

public:
    
    TPathFinderNNSampleSetMultiPart() : TNNSampleSetBaseMultiPart<T>(){
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

