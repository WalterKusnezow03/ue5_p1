#pragma once

#include "CoreMinimal.h"

/// @brief saves a scaling task for Widget Slate Wrapper Base: needed to
/// fix racing conditions. Size handled on tick. Needs to be ticked. Every Widget has to
/// be ticked. Very Important.
struct CUSTOMUIPLUGIN_API FResolutionUpdateTask {

    FResolutionUpdateTask(){
        scaleToSet.X = 0.0f;
        scaleToSet.Y = 0.0f;
        taskDirty = false;
    }
    ~FResolutionUpdateTask(){}

    FResolutionUpdateTask(FVector2D &scale){
        Update(scale);
    }

    FVector2D scaleToSet;
    bool taskDirty = false;

    void Update(FVector2D scale){
        scaleToSet = scale;
        taskDirty = true;
    }

    bool MarkedDirty(){
        bool copy = taskDirty;
        taskDirty = false;
        return copy;
    }
};