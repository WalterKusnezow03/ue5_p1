#pragma once

#include "CoreMinimal.h"


/// @brief simple proxy for loading screen, seperation of concerns for plugins.
class NNCOMMUNICATIONPLUGIN_API LoadingScreenDispatcher {

public:
    static void UpdateLoadingEntry(
        FString tag,
        FString message);

    static void HideLoadingEntry(FString tag);
};