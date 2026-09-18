#pragma once

#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"
#include "PolygonPlugin/Public/Polygons/MergedPolygonRaytracable/MeshedPolygonRaytracable.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/Container/Trajectory.h"


#include "StoragePlugin/Storage/ImageData/Image/Image.h"
#include "PathfinderNNExtension/DataCollection/TrajectoryCollection/MeshedPolygonExtension/Color/MeshedPolygonColorAttributes.h"
#include "PathfinderNNExtension/GameNN/ModelInput/FONNXModelInput.h"

#include "EPolygonSampleType.h"


class FVisionCone;
class FMeshedPolygonColorAttributes;
class IPathfinderNNInterface;

//INTERFACE
class PATHFINDERNNEXTENSION_API FMeshedPolygonTrajectoryLayeredInterface : public FMeshedPolygonRaytracable {

public:
    //custom polymorphism

    virtual EPolygonSampleType GetType(){
        return EPolygonSampleType::none;
    }

public:
    virtual void Reset() = 0;
    virtual void EmbedEnemyVision(const TArray<FVisionCone *> &cones) = 0;
    virtual void EmbedTrajectories(TArray<Trajectory> &trajectories) = 0;
    virtual void EmbedResultPosition(FVector &position) = 0;

    // ---- REQUEST TO NN SIMPLE ACCESS ----
    //prepare data (for netB resize 144 for example)
    virtual void PrepareFitData() {};

    virtual bool PrepareAppendRequestBinary(TArray<uint8> &buffer) = 0;
    virtual bool PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer) = 0;

    //onnx input
    virtual bool PrepareAppendRequestBinary(FONNXModelInput &input) = 0;

    //rename to ResultDataSizeBytes, not grid.
    virtual int ResultDataSizeBytes() = 0; //ResultDataSizeBytes()

    virtual void AppendGroundTruth(TArray<uint8> &buffer) = 0;
    // ---- REQUEST TO NN SIMPLE ACCESS ----

    // ---- process result for game ----
    virtual void ProcessFromPredictionBytes(const TArray<uint8> &buffer) = 0;
    virtual void ProcessFromPredictionFloats(const TArray<float> &buffer) = 0;

    virtual void NotifyVisiblePositionsFor(
        IPathfinderNNInterface *interfaceIn,
        bool useVisiblity
    ) = 0;
    // ---- process result for game ----

    // ---- post process debug images ----
    virtual void ColoredHeatMap(
        Image &image,
        FMeshedPolygonColorAttributes &attributes
    ){};

    // expose all channels as indivual layers
    virtual void ColoredLayersMap(
        TArray<Image> &image,
        FMeshedPolygonColorAttributes &attributes
    ){};



    // ---- Storage Interface ----
    //virtual void AppendAsBinary(TArray<uint8> &buffer) ;
    //virtual bool LoadFromBinary(TArray<uint8> &buffer, uint8 *&Ptr) = 0;



    //must be converted with static cast in derived class
    bool IsSimilar(FMeshedPolygonTrajectoryLayeredInterface &other, float lossMax){
        float similarityMin = 1.0f - lossMax;
        float similarity = SimilarityOfSample(other);
        return similarity >= similarityMin;
    }

    //to be implemented!
    virtual float SimilarityOfSample(FMeshedPolygonTrajectoryLayeredInterface &other) = 0;



protected:

    template <typename T>
    T *TSampleCast(FMeshedPolygonTrajectoryLayeredInterface &other){
        T temp;
        if (temp.GetType() == other.GetType()){
            T* derived_ptr = static_cast<T*>(&other); // Downcast (unchecked)
            return derived_ptr;
        }
        return nullptr;
    }

private:


};
