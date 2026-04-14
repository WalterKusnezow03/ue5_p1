#pragma once

#include "CoreMinimal.h"

class PYTHONPLUGIN_API FPythonSetup {

    //plugin dir
public:
    void Setup(
        FString pluginName,
        FString pyName
    );

    FString getPath() const {
        return pythonPathInPlugin;
    }
    FString PluginDir() const {
        return pluginDirSaved;
    }

private:
    FString PluginDir(FString pluginName);
    FString PythonPath(FString pluginName, FString pyName);

    //something like 
    // pluginName/Source/pluginName/Python
    FString pythonPathInPlugin;
    FString pluginDirSaved;
};