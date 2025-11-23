#include "BufferPackage.h"

BufferPackage::BufferPackage(){
    markedStarted = false;
    asyncCompleted = true;
}

BufferPackage::~BufferPackage(){

}



BufferPackage::BufferPackage(const BufferPackage &other){
    if(this != &other){
        *this = other;
    }
}


BufferPackage &BufferPackage::operator=(const BufferPackage &other){
    if(this != &other){
        asyncCompleted = other.asyncCompleted;
        dataPtr = other.dataPtr;
    }
    return *this;
}


void BufferPackage::MarkLaunchCopy(){
    markedStarted = true;
    asyncCompleted = false;
}


bool BufferPackage::ProcessHasFinished(){
    if(!markedStarted){
        return true; //debug first time copy.
    }

    //async call completed
    if(markedStarted && asyncCompleted){
        markedStarted = false;
        asyncCompleted = false;
        return true;
    }
    return false;
}



FString BufferPackage::flags(){
    int a = markedStarted ? 1 : 0;
    int b = asyncCompleted ? 1 : 0;
    FString message = FString::Printf(TEXT("BufferPackage started(%d) processed(%d)"), a, b);
    return message;
}