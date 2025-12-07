#include "RansacMatcher.h"
#include "ComputerVisionPlugin/Public/ComputerVision/SiftFeatureExtraction/ImageFeatureFinder.h"


RansacMatcher::RansacMatcher(){

}

RansacMatcher::~RansacMatcher(){

}

bool RansacMatcher::Match(
    ImageFeatureFinder &a, 
    ImageFeatureFinder &b,
    float lossmax
){
    TArray<ImagePatch> &patchesA = a.ExtractedFeaturePatches();
    TArray<ImagePatch> &patchesB = b.ExtractedFeaturePatches();

    //sorted by distance for patchesA and B
    int k = 5;
    TArray<PotentialMatch> indexPairsByDistance;
    MakekNearestNeigbor(
        patchesA,
        patchesB,
        indexPairsByDistance,
        k
    );

    float epsilon = 0.75f;
    if(indexPairsByDistance.Num() < 2){
        return false;
    }

    //RemoveBadMatches(indexPairsByDistance, epsilon); //Lowe Ratio Tes
    if(indexPairsByDistance.Num() < 2){
        return false;
    }

    return Train(indexPairsByDistance, lossmax);
}

void RansacMatcher::MakekNearestNeigbor(
    TArray<ImagePatch> &patchesA,
    TArray<ImagePatch> &patchesB,
    TArray<PotentialMatch> &indexPairsByDistance,
    int k
){
    int kLimit = std::min(k, std::max(patchesA.Num(), patchesB.Num()));
    indexPairsByDistance.SetNum(kLimit); //auto constructs elements.

    //fill first elements.
    for (int i = 0; i < kLimit; i++)
    {
        ImagePatch &ithPatch = patchesA[i];
        ImagePatch &jthPatch = patchesB[i];
        float distance = ithPatch.DistanceSquared(jthPatch);
        PotentialMatch &match = indexPairsByDistance[i];

        FVector2D &positionA = ithPatch.GetOriginalImagePosition();
        FVector2D &positionB = jthPatch.GetOriginalImagePosition();
        match.Setup(i, i, distance, positionA, positionB);
    }
    indexPairsByDistance.Sort([](const PotentialMatch &A, const PotentialMatch &B){ 
        return A.Distance() < B.Distance();
    });
    

    // k shifts at worst(?), sort afterwards, nlogn better that n^2 shifts.
    for (int i = 1; i < kLimit; i++)
    {
        ImagePatch &ithPatch = patchesA[i];
        for (int j = i; j < kLimit; j++)
        {
            ImagePatch &jthPatch = patchesB[j];
            float distance = ithPatch.DistanceSquared(jthPatch);
            
            
            for (int inner = 0; inner < indexPairsByDistance.Num(); inner++)
            {
                PotentialMatch &compare = indexPairsByDistance[inner];

                // if lower than prev
                // pop back, insert front
                if (distance < compare.Distance())
                {
                    indexPairsByDistance.Pop();

                    // TArray<int32> Arr = { 1, 2, 3 };
                    // Arr.Insert(99, 1);   // Insert 99 at index 1
                    //  Result: { 1, 99, 2, 3 }
                    FVector2D &positionA = ithPatch.GetOriginalImagePosition();
                    FVector2D &positionB = jthPatch.GetOriginalImagePosition();
                    PotentialMatch newMatch(i, j, distance, positionA, positionB);

                    indexPairsByDistance.Insert(newMatch, inner); //might be -1. NOT TESTED
                }
            }
        }
    }
}


//noch unklar. Warum funktioniert das.
void RansacMatcher::RemoveBadMatches(TArray<PotentialMatch> &matches, float epsilon){
    if(matches.Num() < 2){
        return;
    }

    TArray<PotentialMatch> goodMatches;
    int i = 0;
    while (i < matches.Num())
    {
        PotentialMatch &current = matches[i];

        // finde alle Matches, die zu demselben Keypoint aus A gehören
        

        // finde ersten zwei Matches für diesen Keypoint
        TArray<PotentialMatch*> candidates;
        
        // Sammle die zwei besten Matches für diesen Keypoint
        int j = i;
        while (j < matches.Num() && matches[j].IndexIisSame(current) && candidates.Num() < 2)
        {
            candidates.Add(&matches[j]);
            j++;
        }

        if (candidates.Num() < 2){
            i++;
            continue; // nicht genug Nachbarn : ignorieren
        }

        float d1 = candidates[0]->Distance(); // kleinster Abstand
        float d2 = candidates[1]->Distance(); // zweitkleinster Abstand
        if(std::abs(d2) < 0.00000000001f){
            float sign = d2 < 0.0f ? -1.0f : 1.0f;
            d2 = 0.00000000001f * sign;
        }

        if (d1 / d2 < epsilon)
        {
            goodMatches.Add(*candidates[0]); //copy best match.
        }

        i++; //i = j ????? wieso????
    }

    matches = goodMatches;
}




//call this function with 3 different matches and pick the best model
Eigen::MatrixXf RansacMatcher::ComputeModel(
    TArray<int> &indices, //indices to compute transform for
    TArray<PotentialMatch> &indexPairsByDistance
){
    //statt mit eine fundamental matrix zu arbeiten nehmen wie
    //lineare regression.

    /*
    other(x,y) = R * in(x,y) + t
    */

    int rows = indexPairsByDistance.Num() * 2;
    int columns = 6;

    // 3x4 Matrix
    Eigen::MatrixXf X(rows, columns);
    X.setZero();

    Eigen::MatrixXf y(rows, 1);
    y.setZero();

    //einträge in matrix eintragen
    int rowCurrent = 0;
    for (int i = 0; i < indices.Num(); i++){
        int pair = indices[i]; //choose index.
        if(pair >= 0 && pair < indexPairsByDistance.Num()){
            int indexA = 0;
            int indexB = 0;
            PotentialMatch &current = indexPairsByDistance[pair];
            FVector2D &posA = current.GetPositionFirst();
            FVector2D &posB = current.GetPositionSecond();
            AddVectorToDesignMatrix(X, posA, rowCurrent);
            AddVectorToGroundTruth(y, posB, rowCurrent);
            rowCurrent += 2;
        }
    }

    /*
    for (int i = 0; i < indexPairsByDistance.Num(); i++)
    {
        int indexA = 0;
        int indexB = 0;
        PotentialMatch &current = indexPairsByDistance[i];
        FVector2D &posA = current.GetPositionFirst();
        FVector2D &posB = current.GetPositionSecond();
        AddVectorToDesignMatrix(X, posA, rowCurrent);
        AddVectorToGroundTruth(y, posB, rowCurrent);
        rowCurrent += 2;
    }*/

    //phi_1 = for x coord |x, y, 0, 0, 1, 0|
    //phi_2 = for y coord |0, 0, x, y, 0, 1|
    //w = |w1, w2, w3, w4, tx, ty|

    //X|phi1, phi2...|^T

    //LMSE 
    //w = X^T (X X^T)^-1 * y
    Eigen::MatrixXf Xt = X.transpose();
    Eigen::MatrixXf XXt = X * Xt;
    Eigen::MatrixXf inv = XXt.inverse();
    Eigen::VectorXf w = Xt * inv * y;

    return w;
}

void RansacMatcher::AddVectorToGroundTruth(
    Eigen::MatrixXf &y, 
    FVector2D &pos,
    int row
){
    y(row, 0) = pos.X;
    y(row + 1, 0) = pos.Y;
}

void RansacMatcher::AddVectorToDesignMatrix(
    Eigen::MatrixXf &X, 
    FVector2D &pos,
    int row
){
    int nextRow = row + 1;
    MakePhiVec1(X, pos, row);
    MakePhiVec2(X, pos, nextRow);
}

void RansacMatcher::MakePhiVec1(
    Eigen::MatrixXf &X, 
    FVector2D &pos, 
    int row
){
    //phi_1 = for x coord |x, y, 0, 0, 1, 0|
    //X(row, column)
    X(row, 0) = pos.X;
    X(row, 1) = pos.Y;

    X(row, 2) = 0;
    X(row, 3) = 0;

    X(row, 4) = 1;
    X(row, 5) = 0;
}

void RansacMatcher::MakePhiVec2(
    Eigen::MatrixXf &X, 
    FVector2D &pos, 
    int row
){
    //phi_2 = for y coord |0, 0, x, y, 0, 1|
    X(row, 0) = 0;
    X(row, 1) = 0;

    X(row, 2) = pos.X;
    X(row, 3) = pos.Y;

    X(row, 4) = 0;
    X(row, 5) = 1;
}

//training find best fit modell
bool RansacMatcher::Train(
    TArray<PotentialMatch> &indexPairsByDistance,
    float maxLoss
){
    //daran dann messen erstmal. Loss ist dann ja geomatrisch über alle weights, qud distance.
    float loss = 100000000000.0f;
    Eigen::MatrixXf bestWeight = Eigen::MatrixXf::Zero(6,1);



    //hier lieber log cosh loss ?

    //xO = xB^T * R * T
    //






    int iterations = 20;
    for (int i = 0; i < iterations; i++){

        //pick random matches
        //compute w

        //compute risidua
        //choose best modell
        TArray<int> indices = ChooseRandom(4, indexPairsByDistance);
        
        
        Eigen::MatrixXf wEstimated = ComputeModel(indices, indexPairsByDistance);
        
        //compute loss
        float lossUpdate = ComputeLoss(indexPairsByDistance, wEstimated);
        if (lossUpdate < loss){
            loss = lossUpdate;
            bestWeight = wEstimated;
        }
    }



    DebugHelper::logMessage(
        FString::Printf(
            TEXT("RansacMatcher::loss %.3f w %.2f %.2f %.2f %.2f %.2f %.2f"), 
            loss,
            bestWeight(0,0), //row col
            bestWeight(0,1),
            bestWeight(0,2),
            bestWeight(0,3),
            bestWeight(0,4),
            bestWeight(0,5)
        )
    );

   

    return loss <= maxLoss;
}

TArray<int> RansacMatcher::ChooseRandom(
    int count, 
    TArray<PotentialMatch> &indexPairsByDistance
){
    TArray<int> nums;

    int i = 0;
    while(i < count){
        int num = std::abs(FMath::Rand());
        num %= indexPairsByDistance.Num();
        if(!nums.Contains(num)){
            nums.Add(num);
            i++;
        }
    }
    return nums;
}



float RansacMatcher::ComputeLoss(
    TArray<PotentialMatch> &indexPairsByDistance,
    Eigen::MatrixXf &wEstimated
){
    //immer bezogen auf vec 1 w = vec 2
    float loss = 0.0f;
    for (int i = 0; i < indexPairsByDistance.Num(); i++)
    {
        PotentialMatch &currentMatch = indexPairsByDistance[i];
        FVector2D &vec1 = currentMatch.GetPositionFirst();
        FVector2D &vec2 = currentMatch.GetPositionSecond();
        loss += ComputeLoss(vec1, vec2, wEstimated);
    }
    return loss;
}

float RansacMatcher::ComputeLoss(
    FVector2D &a,
    FVector2D &b,
    Eigen::MatrixXf &wEstimated
){
    //immer bezogen auf vec 1 w = vec 2
    return FVector2D::DistSquared(b, TransformVector(wEstimated, a));
}

FVector2D RansacMatcher::TransformVector(
    Eigen::MatrixXf &w,
    FVector2D &pos
){
    Eigen::MatrixXf X(2, 6); //6 colums, 2 rows
    X.setZero();
    //hinzufügen der einen zeilen
    AddVectorToDesignMatrix(
        X,
        pos,
        0 // row 0
    );
    FVector2D result;
    Eigen::VectorXf resultEigen = X * w;
    result.X = resultEigen(0, 0); //0 row, 0 col
    result.Y = resultEigen(1, 0); //1 row, 0 col

    return result;
}
