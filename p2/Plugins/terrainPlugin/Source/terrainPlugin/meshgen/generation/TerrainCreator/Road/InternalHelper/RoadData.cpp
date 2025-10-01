#include "RoadData.h"


RoadData::RoadData(){

}

RoadData::RoadData(const RoadData &other){
    if(this != &other){
        *this = other;
    }
}

RoadData::RoadData(int id, const TVector<FVector2D> &curveIn){
    roadId = id;
    curve = curveIn;
}

RoadData& RoadData::operator=(RoadData&& other) noexcept {
    if (this != &other) {
        roadId = std::move(other.roadId);
        curve  = std::move(other.curve);
        other.roadId = 0;
    }
    return *this;
}

RoadData &RoadData::operator=(const RoadData &other){
    if(this != &other){

    }
    return *this;
}

RoadData::~RoadData(){

}

int RoadData::Id(){
    return roadId;
}


void RoadData::FindIntersections(RoadData &other, RoadIntersectionMapped &outIntersections){
    FindIntersections(
        curve,
        other.curve,
        other.Id(),
        outIntersections
    );
}

//find intersctions
void RoadData::FindIntersections(
    TVector<FVector2D>& curveA, 
    TVector<FVector2D>& curveB,
    int idOther,
    RoadIntersectionMapped &outIntersections
){
    //will have a runtime of O(n^2)
    for (int i = 1; i < curveA.size(); i++){
        for (int j = 1; j < curveB.size(); j++){
            const FVector2D &v0 = curveA[i-1];
            const FVector2D &v1 = curveA[i];

            const FVector2D &d0 = curveB[j-1];
            const FVector2D &d1 = curveB[j];

            FVector location;
            if (DoesIntersect(v0, v1, d0, d1, location))
            {
                FVector2D location2D(location.X, location.Y);

                std::pair<int, int> intersectionIndexPair;
                intersectionIndexPair.first = i - 1;
                intersectionIndexPair.second = j - 1;


                RoadIntersection intersection;
                intersection.Setup(
                    Id(),
                    idOther,
                    intersectionIndexPair,
                    location2D
                );
                outIntersections.Add(intersection);

            }
        }
    }
}

bool RoadData::DoesIntersect(
    const FVector2D &v0, 
    const FVector2D &v1,
    const FVector2D &d0,
    const FVector2D &d1,
    FVector &IntersectionPoint
){
    //FVector IntersectionPoint;
    //FMath::SegmentIntersection2D(Segment1Start, Segment1End, Segment2Start, Segment2End, IntersectionPoint);
    return FMath::SegmentIntersection2D(
        FVector(v0,0.0),
        FVector(v1,0.0),
        FVector(d0,0.0),
        FVector(d1,0.0),
        IntersectionPoint
    );
}
