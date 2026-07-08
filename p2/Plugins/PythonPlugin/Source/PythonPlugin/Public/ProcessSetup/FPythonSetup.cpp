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
    FString pyname,
    FString pluginNameToPython,
    FString pythonExePath
){
    SetupPythonScript(pluginName, pyname);
    SetupPythonDir(pluginNameToPython, pythonExePath);
}

void FPythonSetup::Setup(
    FString pluginName,
    FString pyName
){
    debugScriptName = pyName;
    pythonPathInPlugin = PythonPath(pluginName, pyName);
}

void FPythonSetup::SetupPythonScript(FString pluginName, FString pyname){
    pluginDirSaved = PluginDir(pluginName);
    //LogTemp: APythonLauncher::PythonPrint( nnpathfinder_server.py ) /opt/homebrew/Cellar/python@3.14/3.14.0_1/Frameworks/Python.framework/Versions/3.14/Resources/Python.app/Contents/MacOS/Python: can't open file 
    //'/Users/walterk/Documents/UnrealProjects/p2/Plugins/PathfinderNNExtension/Source/PathfinderNNExtension/PathFinderNNExtension/Python/nnpathfinder_server.py': [Errno 2] No such file or directory

    FString pythonDir = PluginPythonDir(pluginDirSaved);
    pythonPathInPlugin = PythonPath(pythonDir, pyname);
    debugScriptName = pyname;
}

void FPythonSetup::SetupPythonDir(FString pluginNameToPython, FString pythonExePath){
    if(false){
        return;
    }

    FString pythonPathOther = PluginPythonDir(PluginDir(pluginNameToPython));
    pythonExe = FPaths::Combine(pythonPathOther, pythonExePath);
}




FString FPythonSetup::PythonPath(FString pluginDirPython, FString pyName){
    FString innerPath = FPaths::Combine(pluginDirPython, pyName);
    DebugHelper::logMessage("FPythonSetup::PythonPath --> ", innerPath);
    return innerPath;
}


FString FPythonSetup::PluginPythonDir(FString pluginDir){
    FString PythonPart = FString::Printf(TEXT("Python/"));
    return FPaths::Combine(pluginDir, PythonPart);
}

FString FPythonSetup::PluginDir(FString pluginName){
    FString inner = pluginName; //refactured. // FString::Printf(TEXT("%s/Source/%s"), *pluginName, *pluginName);
    FString pluginDir = FPaths::ConvertRelativePathToFull(
        FPaths::ProjectPluginsDir() / 
        *inner
    );
    DebugHelper::logMessage("FPythonSetup::PluginDir --> ", pluginDir);
    return pluginDir;
}
    