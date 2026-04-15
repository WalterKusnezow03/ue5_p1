#pragma once

#include "CoreMinimal.h"

/// @brief finds a python script from:
/// pluginName/Source/pluginName/Python/some.py
class PYTHONPLUGIN_API FPythonSetup {

public:
    //setups like in class description
    void Setup(
        FString pluginName, //pluginName
        FString pyName //some.py
    );

    FString getPath() const {
        return pythonPathInPlugin;
    }
    FString PluginDir() const {
        return pluginDirSaved;
    }

    FString PythonScriptName() const {
        return debugScriptName;
    }

private:
    FString PluginDir(FString pluginName);
    FString PythonPath(FString pluginName, FString pyName);

    //something like 
    // pluginName/Source/pluginName/Python
    FString pythonPathInPlugin;
    FString pluginDirSaved;

    FString debugScriptName;
};