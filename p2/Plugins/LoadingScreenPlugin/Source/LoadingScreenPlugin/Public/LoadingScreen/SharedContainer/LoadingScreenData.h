#pragma once

#include "CoreMinimal.h"
#include <map>

class LOADINGSCREENPLUGIN_API LoadingScreenData {
    

public:
    FString GetText();
    void UpdateLoadingEntry(FString tag, FString text);
    void HideLoadingEntry(FString tag);
    bool ShowLoadingScreen();

private:
    std::map<FString, FString> textMap;
    std::map<FString, bool> enabledMap;
};