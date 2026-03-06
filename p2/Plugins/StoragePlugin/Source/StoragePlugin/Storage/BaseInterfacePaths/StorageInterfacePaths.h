#pragma once

#include "CoreMinimal.h"

/// @brief base paths - might be changed when building the game (replacing "FPaths::ProjectSavedDir()")
class STORAGEPLUGIN_API StorageInterfacePaths{

public:
    /// @brief will create a path with the world string, ends with a slash "/": somepath/-appendyourinnerPath-
    FString BaseDir(FString worldLevelName){
        FString preString = BaseDir(); //ends with a single "/"
        preString += FString::Printf(TEXT("%s/"), *worldLevelName); //also ends with a single "/"
        return preString;
    }

    /// @brief will create a path within the project save dir "/" at end.
    FString BaseDir(){
        return FPaths::ProjectSavedDir() //has "/" at end
        + TEXT("StorageInterfaceSaved/");
    }


};
