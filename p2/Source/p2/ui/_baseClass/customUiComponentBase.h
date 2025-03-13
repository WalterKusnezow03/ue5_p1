#pragma once

#include "Components/Widget.h"

class UPlayerUi;

/**
 * base functionality each custom ui element should have 
 * (the pointer to the main owning UPlayerUi instance)
 */
class P2_API customUiComponentBase{

public:
    customUiComponentBase(){}
    ~customUiComponentBase(){
        playerUiParent = nullptr;
    }

    virtual void setVisible(bool visible){}

    void saveParent(UPlayerUi &ref){
        playerUiParent = &ref;
    }

    virtual UWidget *baseLayoutPointer(){
        return nullptr;
    }

protected:
    UPlayerUi *playerUiParent = nullptr;

    virtual bool correctInitialized(){
        return playerUiParent != nullptr;
    }

    void setVisible(UWidget *any, bool visible){
        if(any != nullptr){
            ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
            any->SetVisibility(newStatus);
        }
    }


};