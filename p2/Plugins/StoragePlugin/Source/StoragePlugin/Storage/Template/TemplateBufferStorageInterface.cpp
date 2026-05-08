#include "TemplateBufferStorageInterface.h"




void TemplateBufferStorageInterface::AppendCount(
    int32 someCount,
    TArray<uint8> &bytes
){
    TAppendSingleValue<int32>(someCount, bytes);
}

void TemplateBufferStorageInterface::LoadCount(
    int32 &someCount,
    uint8 *& Ptr
){
    TLoadSingleValue<int32>(someCount, Ptr);
}

bool TemplateBufferStorageInterface::EndReached(
    uint8 *ptr,
    TArray<uint8> &bytes
){
    if(ptr){
        uint8 *end = EndPtr(bytes);
        return ptr >= end; //moving oob and equals
    }
    return true;
}


uint8 *TemplateBufferStorageInterface::EndPtr(TArray<uint8> &bytes){
    uint8 *Ptr = bytes.GetData();
    Ptr += bytes.Num(); //move to end
    return Ptr;
}



