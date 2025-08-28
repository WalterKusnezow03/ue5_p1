#pragma once

#include "CoreMinimal.h"

/**
 * acts as a task interface for plugins - processed in Sub Game Project!
 */
class GAMECORE_API worldLevelBase {

public:
    static void addOutpostAt(FVector &location);

    static bool DebugSkelletonRecordMode(){
        return false;
    }

    ///@brief sets the world pointer, can be set in gamemode base subclass 
    ///and resetet on end play if a level switches (which i dont want)
    ///or on end game 
    static void SetWorld(UWorld *world);
    static void ResetWorldPointer();
    static UWorld *GetWorld();

    static void EndPlay();

protected:
    static TArray<FVector> outpostsToCreate;

private:
    static UWorld *worldPointer;

    
};