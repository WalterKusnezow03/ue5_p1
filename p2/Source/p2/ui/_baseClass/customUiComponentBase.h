#pragma once

#include "Components/Widget.h"

#include "customUiComponentBase.generated.h"

class UPlayerUi;

/**
 * base functionality each custom ui element should have 
 * (the pointer to the main owning UPlayerUi instance)
 */
UCLASS()
class P2_API UcustomUiComponentBase : public UObject{

    GENERATED_BODY()

public:
    virtual void init(){
        WAS_INIT_FLAG = true;
    }
    virtual void init(UPlayerUi &refin){
        saveParent(refin);
        WAS_INIT_FLAG = true;
    }
    virtual void setVisible(bool visible) {}

    void saveParent(UPlayerUi &ref){
        playerUiParent = &ref;
    }

    //MUST BE OVERRIDEN!
    virtual UWidget *baseLayoutPointer(){
        return nullptr;
    }

protected:
    UPlayerUi *playerUiParent = nullptr;

    bool WAS_INIT_FLAG = false;

    void setVisible(UWidget *any, bool visible){
        if(any != nullptr){
            //ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

            ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
            // ESlateVisibility::Hidden;
            any->SetVisibility(newStatus);

            //test
            any->SetIsEnabled(visible);
        }
    }


    void setVisibleNoCollsion(UWidget *any, bool visible){
        if(any){

            //disbale collsion
            ESlateVisibility visiblity = visible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
            any->SetVisibility(visiblity);
        }
    }



};