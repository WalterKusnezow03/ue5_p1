#pragma once

#include "CoreMinimal.h"
#include "DebugPlugin/DebugHelper.h"

class PATHFINDERNNEXTENSION_API NNPluginSettings{

public:
    static bool UsePythonNN(){

        #if PLUGIN_GLOBAL_FLAG_USE_PYTHON_NN
            return true;
        #else
            return false;
        #endif

        return true;
        // PLUGIN_GLOBAL_FLAG_USE_PYTHON_NN
    }

    static void LogSetting(){
        FString enabled = UsePythonNN() ? " PYTHON " : " ONNX ";

        DebugHelper::logMessage("NNSETTINGS: ", enabled);
    }
};