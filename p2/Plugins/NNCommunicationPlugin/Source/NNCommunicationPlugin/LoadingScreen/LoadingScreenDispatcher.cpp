#include "LoadingScreenDispatcher.h"
#include "LoadingScreenPlugin/Public/LoadingScreen/LoadingScreenGameInstance.h"

void LoadingScreenDispatcher::UpdateLoadingEntry(
    FString tag,
    FString message 
){
    ULoadingScreenGameInstance::UpdateLoadingEntrySlateTick(tag, message);
}

void LoadingScreenDispatcher::HideLoadingEntry(FString tag){
    ULoadingScreenGameInstance::HideLoadingEntrySlateTick(tag);
}