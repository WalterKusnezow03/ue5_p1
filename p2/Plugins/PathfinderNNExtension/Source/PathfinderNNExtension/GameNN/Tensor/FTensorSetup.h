#pragma once

#include "CoreMinimal.h"


class PATHFINDERNNEXTENSION_API FTensorSetup {

protected:
    int tensorWidth = 1;
    int tensorHeight = 1;
    int channels = 1;

public:
   
    virtual void Setup(int widthIn, int heightIn, int channelsIn){
        tensorWidth = widthIn;
        tensorHeight = heightIn;
        channels = channelsIn;
    }

    int GetWidth() const {
        return tensorWidth;
    }
    int GetHeight() const {
        return tensorHeight;
    }
    int GetChannels() const {
        return channels;
    }

};
