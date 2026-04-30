#include "FSharedFrameManager.h"
#include "DebugPlugin/DebugHelper.h"

FSharedFrameManager::FSharedFrameManager(){

}

FSharedFrameManager::~FSharedFrameManager(){
    ClearAllFrames();
}

void FSharedFrameManager::ClearAllFrames(){
    TArray<FSharedFrame *> outArray = allFrames();
    for (int i = 0; i < outArray.Num(); i++){
        if(FSharedFrame *current = outArray[i]){
            current->CleanFrame();
        }
    }
    
    frames.clear();
}
TArray<FSharedFrame *> FSharedFrameManager::allFrames(){
    TArray<FSharedFrame *> outArray;
    for(auto &pair : frames){
        outArray.Add(&pair.second);
    }
    return outArray;
}

void FSharedFrameManager::Open(FString name, int bytes){
    DebugHelper::logMessage(
        FString::Printf(TEXT("FSharedFrameManager::OpenFrame Try %s %d"), *name, bytes)
    );


    if(bytes > 0 && name.Len() > 0){
        
        /*if(FSharedFrame *frame = FindFrame(name)){
            if (frame->SizeChanged(bytes * sizeof(uint8))){
                frame->CleanFrame();
            }
        }*/
        if(!HasFrame(name)){
            bool closeOnDestroy = false; //do not change: copy constructor destroy is bad.
            frames[name] = FSharedFrame(closeOnDestroy); //do not auto close!!!
        }
        
        FSharedFrame &ref = frames[name];
        ref.Open(name, bytes);
        DebugHelper::logMessage(
            FString::Printf(TEXT("FSharedFrameManager::OpenFrame %s %d"), *name, bytes)
        );
        
    }
}   

bool FSharedFrameManager::HasFrame(FString pageName){
    return frames.find(pageName) != frames.end();
}

FSharedFrame *FSharedFrameManager::FindFrame(FString name){
    if(HasFrame(name)){
        return &frames[name];
    }
    return nullptr;
}

void FSharedFrameManager::WriteData(FString pageName, const TArray<uint8> &data, bool &frameChanged){
    DebugHelper::logMessage(
        FString::Printf(TEXT("FSharedFrameManager::Write Data Try %s"), *pageName)
    );
    int bytes = data.Num();
    if(bytes > 0){
        if (!HasFrame(pageName)){
            Open(pageName, bytes);
            frameChanged = true;
        }else{
            FSharedFrame &frame = frames[pageName];
            if (frame.SizeChanged(bytes * sizeof(uint8))){
                frame.CleanFrame();
                Open(pageName, bytes);
                frameChanged = true;
            }
        }
        frames[pageName].WriteData(data);
        DebugHelper::logMessage(
            FString::Printf(TEXT("FSharedFrameManager::Write Data End %s %d"), *pageName, bytes)
        );
    }
}