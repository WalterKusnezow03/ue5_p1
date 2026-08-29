#include "ExternalActorSpawnPositions.h"
#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"
#include "StoragePlugin/Storage/StringData/StorageInterfaceStringArray.h"

ExternalActorSpawnPositions::ExternalActorSpawnPositions(){

}

ExternalActorSpawnPositions::~ExternalActorSpawnPositions(){

}

ExternalActorSpawnPositions::ExternalActorSpawnPositions(FString nameIn){
    SetName(nameIn);
}

ExternalActorSpawnPositions::ExternalActorSpawnPositions(FString nameIn, float onePerCmAreaIn){
    SetName(nameIn);
    SetAxisScalePerCm(onePerCmAreaIn);
}

ExternalActorSpawnPositions::ExternalActorSpawnPositions(FString nameIn, int32 onePerCmAreaIn){
    SetName(nameIn);
    SetAxisScalePerCm(onePerCmAreaIn);
}

void ExternalActorSpawnPositions::SetName(FString name){
    nameTagActor = name;
}

FString ExternalActorSpawnPositions::GetName() const {
    return nameTagActor;
}

const TArray<FVector> &ExternalActorSpawnPositions::GetSpawnPositions() const {
    return outPositions;
}

bool ExternalActorSpawnPositions::NameEqualsIgnoreCase(FString other){
    if(IsValid()){
        return nameTagActor.Equals(other, ESearchCase::IgnoreCase);
    }
    return false;
}

void ExternalActorSpawnPositions::AddPosition(FVector posIn){
    outPositions.Add(posIn);
}
   
bool ExternalActorSpawnPositions::IsValid(){
    return IsValidName(nameTagActor);
}

bool ExternalActorSpawnPositions::IsValidName(const FString &name){
    return name.Len() > 0;
}

/// append as binary

void ExternalActorSpawnPositions::AppendAsBinary(
    TArray<uint8> &buffer
){
    if(IsValid()){

        StorageInterfaceStringArray::AppendString(buffer, nameTagActor);

        //append onePerCmArea (int)
        TemplateBufferStorageInterface::AppendCount(onePerCmArea, buffer);

        TemplateBufferStorageInterface::TAppendBuffer<FVector>(
            outPositions,
            buffer
        );
    }
}



bool ExternalActorSpawnPositions::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *& Ptr //reference to a pointer. Pointer by reference.
){

    if(TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
        return false;
    }

    nameTagActor = StorageInterfaceStringArray::loadString(Ptr);

    //load count (onePerCmArea)
    TemplateBufferStorageInterface::LoadCount(onePerCmArea, Ptr);

    TemplateBufferStorageInterface::TLoadBuffer(
        outPositions,
        Ptr
    );

    return true;

}



int ExternalActorSpawnPositions::GetAxisScalePerCm(){
    return onePerCmArea;
}

void ExternalActorSpawnPositions::SetAxisScalePerCm(int32 cmIn){
    onePerCmArea = std::abs(cmIn);
    onePerCmArea = std::max(onePerCmArea, 1);
}

void ExternalActorSpawnPositions::SetAxisScalePerMeter(int32 meterIn){
    SetAxisScalePerCm(meterIn * 100);
}