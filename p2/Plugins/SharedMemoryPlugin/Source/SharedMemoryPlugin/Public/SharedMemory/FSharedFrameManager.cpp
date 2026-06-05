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
        /*if (!HasFrame(pageName)){
            Open(pageName, bytes);
            frameChanged = true;
        }else{
            FSharedFrame &frame = frames[pageName];
            if (frame.SizeChanged(bytes * sizeof(uint8))){
                frame.CleanFrame();
                Open(pageName, bytes);
                frameChanged = true;
            }
        }*/
        MakeSureExists(pageName, bytes, frameChanged);
        frames[pageName].WriteData(data);
        DebugHelper::logMessage(
            FString::Printf(TEXT("FSharedFrameManager::Write Data End %s %d"), *pageName, bytes)
        );
    }
}





void FSharedFrameManager::ReadData(
    FString pageName, 
    TArray<uint8> &data, 
    int bytesToRead,
    bool &frameChanged
){
    data.Empty();
    DebugHelper::logMessage(
        FString::Printf(TEXT("FSharedFrameManager::Read Data Try %s"), *pageName)
    );
    if(bytesToRead > 0){
        /*if (!HasFrame(pageName)){
            Open(pageName, bytesToRead);
            frameChanged = true;
        }else{
            FSharedFrame &frame = frames[pageName];
            if (frame.SizeChanged(bytes * sizeof(uint8))){
                frame.CleanFrame();
                Open(pageName, bytesToRead);
                frameChanged = true;
            }
        }*/
        MakeSureExists(pageName, bytesToRead, frameChanged);
        //frames[pageName].ReadData(data);
        TryReadDataTo(pageName, data);

        DebugHelper::logMessage(
            FString::Printf(TEXT("FSharedFrameManager::Read Data End %s %d"), *pageName, data.Num())
        );
    }
}


void FSharedFrameManager::TryReadDataTo(FString pageName, TArray<uint8> &buffer){
    if(HasFrame(pageName)){
        frames[pageName].ReadData(buffer);
    }
}


bool FSharedFrameManager::TryReadReadyFlag(FString pageName){
    if(HasFrame(pageName)){
        bool result = frames[pageName].TryReadReadyFlag();
        FString messageA = "FSharedFrameManager::TryReadReadyFlag " + pageName;
        FString messageB = result ? " True " : " False";
        //DebugHelper::logMessage(messageA, messageB);
        return result;
    }
    return false;
}

void FSharedFrameManager::MarkReadyFalse(FString pageName){
    FString messageA = "FSharedFrameManager::MarkReadyFlagFalse TRY " + pageName;
    DebugHelper::logMessage(messageA);
    if(HasFrame(pageName)){
        messageA = "FSharedFrameManager::MarkReadyFlagFalse " + pageName;
        DebugHelper::logMessage(messageA);
        frames[pageName].MarkReadyFalse();
        //debug
        TryReadReadyFlag(pageName);
    }
}



void FSharedFrameManager::MakeSureExists(FString pageName, int bytes, bool &frameChanged){
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
        if(frameChanged){
            DebugHelper::logMessage(
                FString::Printf(TEXT("FSharedFrameManager::MakeSureExists End %s %d"), *pageName, bytes)
            );
        }
    }
}