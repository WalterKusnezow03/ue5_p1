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