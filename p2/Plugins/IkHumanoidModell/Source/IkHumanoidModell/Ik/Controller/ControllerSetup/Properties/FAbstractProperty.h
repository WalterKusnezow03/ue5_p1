#pragma once

class IKHUMANOIDMODELL_API FAbstractProperty {

public:
    float Validate(float size){
        return std::max(std::abs(size), 1.0f);
    }

    virtual void SetWorld(UWorld *worldIn){
        worldPtr = worldIn;
    }

    UWorld *GetWorld(){
        return worldPtr;
    }

protected:
    UWorld *worldPtr = nullptr;
};