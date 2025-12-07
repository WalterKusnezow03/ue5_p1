#pragma once

#include "CoreMinimal.h"
#include "PotentialMatch.h"
#include "ComputerVisionPlugin/Public/ComputerVision/SiftFeatureExtraction/ImagePatch.h"
#include <Eigen/Dense>

class ImageFeatureFinder;


///Descriptor matching RANSAC
class COMPUTERVISIONPLUGIN_API RansacMatcher {

public:
    RansacMatcher();
    ~RansacMatcher();

    bool Match(ImageFeatureFinder &a, ImageFeatureFinder &b, float lossmax);

private:
    TArray<FVector2D> matchesImage1;
    TArray<FVector2D> matchesImage2;

    void MakekNearestNeigbor(
        TArray<ImagePatch> &patchesA,
        TArray<ImagePatch> &patchesB,
        TArray<PotentialMatch> &indexPairsByDistance,
        int k
    );
    

    void RemoveBadMatches(TArray<PotentialMatch> &matches, float epsilon);
    
    
    
    
    
    Eigen::MatrixXf ComputeModel(
        TArray<int> &indices,
        TArray<PotentialMatch> &indexPairsByDistance
    );

    void AddVectorToGroundTruth(
        Eigen::MatrixXf &y,
        FVector2D &pos,
        int row
    );
    void AddVectorToDesignMatrix(Eigen::MatrixXf &X, FVector2D &pos, int row);
    void MakePhiVec1(
        Eigen::MatrixXf &X,
        FVector2D &pos,
        int row
    );
    void MakePhiVec2(
        Eigen::MatrixXf &X,
        FVector2D &pos,
        int row
    );

    bool Train(
        TArray<PotentialMatch> &indexPairsByDistance,
        float maxLoss
    );

    TArray<int> ChooseRandom(
        int count,
        TArray<PotentialMatch> &indexPairsByDistance
    );


    float ComputeLoss(
        TArray<PotentialMatch> &indexPairsByDistance,
        Eigen::MatrixXf &wEstimated
    );
    
    float ComputeLoss(
        FVector2D &a,
        FVector2D &b,
        Eigen::MatrixXf &wEstimated
    );

    FVector2D TransformVector(
        Eigen::MatrixXf &w,
        FVector2D &pos
    );
};