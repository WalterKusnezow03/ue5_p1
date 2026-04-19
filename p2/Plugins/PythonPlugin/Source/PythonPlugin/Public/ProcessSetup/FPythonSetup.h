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

    void Setup(
        FString pluginName,
        FString pyname,
        FString pluginNameToPython,
        FString pythonExePath //pluginname/source/pluginname/Python/path/to/python.bin
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

    FString PythonExe() const {
        return pythonExe;
    }

private:
    FString PluginDir(FString pluginName);
    FString PythonPath(FString pluginName, FString pyName);
    FString PluginPythonDir(FString pluginDir);

    void SetupPythonScript(FString pluginName, FString pyname);
    void SetupPythonDir(FString pluginNameToPython, FString pythonExePath);

    //something like 
    // pluginName/Source/pluginName/Python
    FString pythonPathInPlugin;
    FString pluginDirSaved;
    FString pluginPythonDirSaved;

    FString pythonExe = TEXT("/opt/homebrew/bin/python3");
    
    

    FString debugScriptName;
};