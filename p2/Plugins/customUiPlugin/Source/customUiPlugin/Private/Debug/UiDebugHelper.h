#pragma once

#include "CoreMinimal.h"

class CUSTOMUIPLUGIN_API UiDebugHelper {

public:
    static void logMessage(FString printing);

private:
    UiDebugHelper() {};
    ~UiDebugHelper() {};
};