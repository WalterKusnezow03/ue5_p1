#include "FPythonSetup.h"

#include "DebugPlugin/DebugHelper.h"

/*
FString PluginDir = FPaths::ConvertRelativePathToFull(
    FPaths::ProjectPluginsDir() / 
    TEXT("NNCommunicationPlugin/Source/NNCommunicationPlugin")
);

FString PythonExe = TEXT("/opt/homebrew/bin/python3");

FString innerPath = FString::Printf(TEXT("Python/%s"), *name);
//FString PythonScript = FPaths::Combine(PluginDir, TEXT("Python/example.py"));
FString PythonScript = FPaths::Combine(PluginDir, innerPath);
*/

void FPythonSetup::Setup(
    FString pluginName,
    FString pyName
){
    debugScriptName = pyName;
    pythonPathInPlugin = PythonPath(pluginName, pyName);
}

FString FPythonSetup::PythonPath(FString pluginName, FString pyName){
    FString innerPath = FString::Printf(TEXT("Python/%s"), *pyName);

    pluginDirSaved = PluginDir(pluginName);
    FString PythonScript = FPaths::Combine(pluginDirSaved, innerPath);

    DebugHelper::logMessage("FPythonSetup::PythonPath --> ", PythonScript);
    return PythonScript;
}

FString FPythonSetup::PluginDir(FString pluginName){
    FString inner = FString::Printf(TEXT("%s/Source/%s"), *pluginName, *pluginName);
    FString pluginDir = FPaths::ConvertRelativePathToFull(
        FPaths::ProjectPluginsDir() / 
        *inner
    );
    DebugHelper::logMessage("FPythonSetup::PluginDir --> ", pluginDir);
    return pluginDir;
}
    