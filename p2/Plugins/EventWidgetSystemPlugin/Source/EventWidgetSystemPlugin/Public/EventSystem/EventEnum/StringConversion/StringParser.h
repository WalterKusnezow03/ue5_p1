#pragma once

#include "CoreMinimal.h"

class EVENTWIDGETSYSTEMPLUGIN_API StringParser {

public:
    TArray<FString> Split(FString message, FString seperator){

        //FString input = "Actor_Widget_Button";
        TArray<FString> parts;
        message.ParseIntoArray(parts, *seperator);
        return parts;
    }
};
