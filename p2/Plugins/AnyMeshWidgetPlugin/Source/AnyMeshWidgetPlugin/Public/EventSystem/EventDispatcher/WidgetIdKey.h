// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"


/// @brief will store a actor name and widget name for event dispatching
class ANYMESHWIDGETPLUGIN_API WidgetIdKey  {
   
public:

  
    //std map override
    bool operator<(const WidgetIdKey& other) const
    {
        if (ActorName < other.ActorName)
            return true;

        if (ActorName > other.ActorName)
            return false;

        return WidgetName < other.WidgetName;
    }

    void Setup(FString actornameIn, FString widgetNameIn){
        ActorName = actornameIn;
        WidgetName = widgetNameIn;
    }

    bool IsValidKey() const {
        return ActorName.Len() > 0 && WidgetName.Len() > 0;
    }

    //setup from event message containing widget + actor id and payload.
    void SetupFromFirstTwoStrings(const TArray<FString> &array){
        if(array.Num() >= 2){
            Setup(array[0], array[1]);
        }
    }

    FString ToString() const {
        return FString::Printf(
            TEXT("WidgetIdKey: %s %s"),
            *ActorName,
            *WidgetName
        );
    }

protected:
    FString ActorName;
    FString WidgetName;


};

