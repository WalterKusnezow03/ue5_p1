#include "FMovementToken.h"
#include "DebugPlugin/DebugHelper.h"

void FMovementToken::Generate(const Trajectory &current, const Trajectory &next){
    FVector posA = current.GetPosition();
    FVector posB = next.GetPosition();
    FVector dir = posB - posA; //nicht normalisiert

    deltaX = dir.X;
    deltaY = dir.Y;
    deltaZ = dir.Z;
    deltaT = next.GetTime() - current.GetTime();
}

void FMovementToken::AppendToBuffer(TArray<uint8> &buffer) const{
    TemplateBufferStorageInterface::TAppendSingleValue<float>(deltaX, buffer);
    TemplateBufferStorageInterface::TAppendSingleValue<float>(deltaY, buffer);
    TemplateBufferStorageInterface::TAppendSingleValue<float>(deltaZ, buffer);
    TemplateBufferStorageInterface::TAppendSingleValue<float>(deltaT, buffer);
}

void FMovementToken::Generate(const TArray<float> &buffer, int lastIndex){
    if(lastIndex >= 0 && lastIndex < buffer.Num()){
        int variables = 4;
        int frontIndex = lastIndex - variables + 1; // variables = 4
        if(frontIndex >= 0){
            Generate(
                buffer[lastIndex - 3],
                buffer[lastIndex - 2],
                buffer[lastIndex - 1],
                buffer[lastIndex]
            );
        }
    }
}

void FMovementToken::Generate(float x, float y, float z, float t){
    deltaX = x;
    deltaY = y;
    deltaZ = z;
    deltaT = t;
}

void FMovementToken::Reset(){
    deltaX = 0.0f;
    deltaY = 0.0f;
    deltaZ = 0.0f;
    deltaT = 0.0f;
}