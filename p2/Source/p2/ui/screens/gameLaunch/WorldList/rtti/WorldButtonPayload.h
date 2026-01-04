#pragma once

#include "customuipluginbase/payload/PayLoadBase.h"

#include "WorldButtonPayload.generated.h"

UCLASS()
class P2_API UWorldButtonPayload : public UPayLoadBase {
    GENERATED_BODY()

public:
    void SetTypeWorldNameButton(){
        isDeleteButton = false;
    }
    void SetTypeRemoveButton(){
        isDeleteButton = true;
    }

    bool IsWorldNameButton(){
        return !IsRemoveButton();
    }

    bool IsRemoveButton(){
        return isDeleteButton;
    }

private:
    bool isDeleteButton = false;
};