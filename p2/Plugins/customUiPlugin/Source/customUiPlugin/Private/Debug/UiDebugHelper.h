#pragma once

#include "CoreMinimal.h"

class CUSTOMUIPLUGIN_API UiDebugHelper {

public:
    static void logMessage(FString printing);
    static void showScreenMessage(FString s, FColor color);

private:
    UiDebugHelper() {};
    ~UiDebugHelper() {};
};