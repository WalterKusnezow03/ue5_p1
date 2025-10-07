#include "RoadSectionList.h"




FRoadSectionList::FRoadSectionList(){

};
FRoadSectionList::~FRoadSectionList(){

};

FRoadSectionList::FRoadSectionList(const FRoadSectionList &other){
    if(this != &other){
        *this = other;
    }
}

FRoadSectionList &FRoadSectionList::operator=(const FRoadSectionList &other){
    if(this != &other){
        sections = other.sections;
    }
    return *this;
}

/// @brief sorts the road sections from 
/// @param other 
void FRoadSectionList::Add(FRoadSection &other){
    sections.Add(other);
}

///ordered in relation to previous section
void FRoadSectionList::AddOrdered(FRoadSection &other){
    if(sections.Num() <= 0){
        Add(other);
        return;
    }

    FRoadSection &last = sections.Last();
    if(last.RoadId() == other.RoadId()){
        int end = last.getEndIndex();
        int deltaStart = other.getStartIndex() - end; // AB = B - A
        int deltaEnd = other.getEndIndex() - end;

        //re order if needed
        if(deltaEnd < deltaStart){
            FRoadSection reOrdered;
            reOrdered.Setup(other.RoadId(), other.getEndIndex(), other.getStartIndex());
            Add(reOrdered);
            return;
        }
    }
    Add(other); //Default add since ordered properly / not concatenated
}





/// @brief 
//void Sort();


//add get all ids here Tarray(ids)
TArray<int> FRoadSectionList::getRoadIds(){
    TArray<int> outIds;
    outIds.SetNum(sections.Num());
    for (int i = 0; i < sections.Num(); i++){
        outIds[i] = sections[i].RoadId();
    }
    return outIds;
}


TArray<FVector2D> &FRoadSectionList::BuildedFromSections(){
    return buildedCombinedSpline; //BuildedFromSections
}

void FRoadSectionList::BuildPolygonAutoExtract(TArray<RoadData> &allRoads){
    
    //order all roads -> and extract the corrosponding parts
    TArray<RoadData *> RoadsOrdered;
    TArray<int> ids = getRoadIds();
    for (int i = 0; i < ids.Num(); i++){
        int idSearched = ids[i];
        for (int j = 0; j < allRoads.Num(); j++){
            RoadData &currentRoad = allRoads[j];
            if(currentRoad.Id() == idSearched){
                RoadsOrdered.Add(&currentRoad); //add ptr for process
            }
        }
    }
    buildedCombinedSpline = BuildPolygonFromOrderedRoads(RoadsOrdered);
}

TArray<FVector2D> FRoadSectionList::BuildPolygonFromOrderedRoads(
    TArray<RoadData *> &RoadsOrdered
){
    TArray<FVector2D> outPolygon;
    for (int i = 0; i < RoadsOrdered.Num(); i++)
    {
        if(RoadData *current = RoadsOrdered[i]){
            if(i < sections.Num()){
                FRoadSection &section = sections[i];
                // FRoadSection::extractPart(RoadData &roadData, TArray<FVector2D> &outAppend);
                section.extractPart(*current, outPolygon);
            }
        }
    }
    return outPolygon; //is copied right now, could be passed by ref!
}