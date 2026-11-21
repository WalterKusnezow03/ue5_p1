#include "PartListStorageInterface.h"



PartListStorageInterface::PartListStorageInterface(){

}

PartListStorageInterface::~PartListStorageInterface(){

}

void PartListStorageInterface::Save(){
    TArray<FString> filtered = makeUniqueNonEmpty(partNames);
    StorageInterfaceStringArray::Save(makePath(), filtered);
}

bool PartListStorageInterface::Load(){
    partNames.Empty();
    return StorageInterfaceStringArray::Load(makePath(), partNames);
}

FString PartListStorageInterface::makePath(){
    FString innerPath = FString::Printf(TEXT("%s_partList.bin"), *fileNameSavedBase);
    FString path = BaseDir("PathFinderNNSamples") + innerPath;
    return path;
}


FString PartListStorageInterface::AddNextFileName(){
    AddFileName(SizeList());
    FString name = MakeFileName(0);
    if(SizeList() > 0){
        name = partNames.Last();
    }
    return name;
}

FString PartListStorageInterface::AddFileNameIfNeeded(int index){
    FString target = MakeFileName(index);
    AddFileName(target);
    return target;
}

void PartListStorageInterface::AddFileName(int index){
    AddFileName(MakeFileName(index));
}

FString PartListStorageInterface::MakeFileName(int index){
    return FString::Printf(TEXT("%s_%d"), *fileNameSavedBase, index);
}


void PartListStorageInterface::AddFileName(FString nameIn){
    if(partNames.Contains(nameIn) == false){
        partNames.Add(nameIn);
        Save();
    }
}

int32 PartListStorageInterface::SizeList(){
    return partNames.Num();
}

TArray<FString> PartListStorageInterface::GetAllPartNames(){
    return partNames;
}


bool PartListStorageInterface::GetNextFileName(FString name, FString &outName) const {

    for (int i = 0; i < partNames.Num(); i++){
        const FString &current = partNames[i];
        if(current.Equals(name)){
            //check if next is possible
            if(IsValidIndex(i+1)){
                outName = partNames[i+1];
                return true;
            }
        }
    }
    return false;
}

bool PartListStorageInterface::IsValidIndex(int i) const {
    return i >= 0 && i < partNames.Num();
}