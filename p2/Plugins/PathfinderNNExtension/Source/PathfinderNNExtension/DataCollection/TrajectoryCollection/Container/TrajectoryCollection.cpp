#include "TrajectoryCollection.h"



TrajectoryCollection::TrajectoryCollection(){

}

TrajectoryCollection::~TrajectoryCollection(){

}

TrajectoryCollection::TrajectoryCollection(const TrajectoryCollection &other){
    if(this != &other){
        *this = other;
    }
}

TrajectoryCollection &TrajectoryCollection::operator=(const TrajectoryCollection &other){
    if(this != &other){
        CopyList(other);
        maxElements = other.maxElements;
    }
    return *this;
}

void TrajectoryCollection::Setup(int maxSize){
    maxSize = std::abs(maxSize);
    maxElements = std::abs(maxSize);
}



void TrajectoryCollection::Add(FVector &pos){
    num++;
    trajectories.AddTail(Trajectory(pos));
    if(num >= maxElements){
        num--;
        PopFront();
    }
}

void TrajectoryCollection::PopFront(){
    if(TDoubleLinkedList<Trajectory>::TDoubleLinkedListNode* head = trajectories.GetHead()){
        trajectories.RemoveNode(head);
    }
}

int TrajectoryCollection::Num()const{
    return num;
}

int TrajectoryCollection::NumMax() const{
    return maxElements;
}

TArray<Trajectory> TrajectoryCollection::ToArray()const{
    TArray<Trajectory> outArray;
    if(num > 0){
        outArray.SetNumUninitialized(num);
        int i = 0;

        const TDoubleLinkedList<Trajectory>::TDoubleLinkedListNode *Node = trajectories.GetHead();
        while(Node != nullptr){
            outArray[i] = Node->GetValue();
            i++;
            Node = Node->GetNextNode();
        }
    }
    MoveToLocalTime(outArray);
    return outArray;
}

TArray<Trajectory> TrajectoryCollection::ToArrayNormalizedTime() const {
    TArray<Trajectory> data = ToArray();
    NormalizeTime(data);
    return data;
}

void TrajectoryCollection::NormalizeTime(TArray<Trajectory> &data) const {
    float max = 0.0f;
    for (int i = 0; i < data.Num(); i++){
        max = data[i].AbsMaxTime(max);
    }
    for (int i = 0; i < data.Num(); i++){
        data[i] /= max;
    }
}

void TrajectoryCollection::MoveToLocalTime(TArray<Trajectory> &outArray) const {
    float timeNow = FPlatformTime::Seconds();
    for (int i = 0; i < outArray.Num(); i++)
    {
        outArray[i] -= timeNow;
    }
}







TArray<Trajectory> TrajectoryCollection::ToArrayLocalSpace(const FVector &worldLocation)const{
    //AB = B - A
    TArray<Trajectory> outArray = ToArray();
    for (int i = 0; i < outArray.Num(); i++){
        outArray[i] -= worldLocation;
    }
    return outArray;
}




void TrajectoryCollection::CopyList(const TrajectoryCollection &other){
    const TDoubleLinkedList<Trajectory>::TDoubleLinkedListNode *Node = other.trajectories.GetHead();
    while(Node != nullptr){
        FVector pos = Node->GetValue().GetPosition();
        Add(pos);

        Node = Node->GetNextNode();
    }
}



//FVector2D dir
bool TrajectoryCollection::EndDir(
    const TArray<Trajectory> &inTrajectories,
    FVector2D &outDir
){
    if(inTrajectories.Num() > 1){
        const Trajectory &last = inTrajectories[inTrajectories.Num() - 1];
        const Trajectory &prelast = inTrajectories[inTrajectories.Num() - 2];
        FVector dir = Dir2D(prelast, last);
        
        outDir.X = dir.X;
        outDir.Y = dir.Y;

        return true;
    }
    return false;
}


bool TrajectoryCollection::GlobalDir(
    const TArray<Trajectory> &inTrajectories,
    FVector2D &outDir
){
    if(inTrajectories.Num() > 1){
        FVector dir;
        for (int i = 1; i < inTrajectories.Num(); i++)
        {
            const Trajectory &current = inTrajectories[i];
            const Trajectory &prev = inTrajectories[i-1];
            dir += Dir2D(prev, current);
        }

        dir = dir.GetSafeNormal();

        outDir.X = dir.X;
        outDir.Y = dir.Y;
        return true;
    }
    return false;
}


FVector TrajectoryCollection::Dir2D(const Trajectory &a, const Trajectory &b){
    FVector B = b.GetPosition();
    FVector A = a.GetPosition();
    FVector dir = B - A;
    dir.Z = 0.0f;
    dir = dir.GetSafeNormal();
    return dir;
}