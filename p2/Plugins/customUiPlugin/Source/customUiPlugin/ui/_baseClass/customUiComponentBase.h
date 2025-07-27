#pragma once

#include "Components/Widget.h"

#include "customUiComponentBase.generated.h"

class UPlayerUiBase;

/**
 * base functionality each custom ui element should have 
 * (the pointer to the main owning UPlayerUi instance)
 */
UCLASS()
class CUSTOMUIPLUGIN_API UcustomUiComponentBase : public UObject{

    GENERATED_BODY()

public:
    // --- CALL ON ANY DERIVED BEFORE USING! ---
    //construct
    virtual void init(){
        WAS_INIT_FLAG = true;
    }
    virtual void init(UPlayerUiBase &refin){
        saveParent(refin);
        WAS_INIT_FLAG = true;
    }


    // --- manual click dispatch ---
    virtual bool dispatchClick(){
        return false;
    }

    /// @brief marks button as invisible: may be needed to not dispatch a click, base layout pointer is 
    /// invisible too!
    /// @param visible 
    virtual void setVisible(bool visible) {
        setVisible(baseLayoutPointer(), visible);
    }

    //--- Ticker section ---
    //destruct
    virtual void BeginDestroy() override
    {
        //remove tick from tick handler
        disableTick();

        Super::BeginDestroy();
    }

    ///@brief is called by tick handler if subscribed
    virtual void Tick(float DeltaTime){
        if(!TICK_ENABLED){
            return;
        }
    }

    //deprecated flag.
    void enableTick(){
        TICK_ENABLED = true;
    }

    void disableTick(){
        TICK_ENABLED = false;
    }

    //--- Ticker section end ---


    void saveParent(UPlayerUiBase &ref){
        playerUiParent = &ref;
    }

    //MUST BE OVERRIDEN!
    virtual UWidget *baseLayoutPointer(){
        return nullptr;
    }

protected:
    UPlayerUiBase *playerUiParent = nullptr;

    bool WAS_INIT_FLAG = false;

    void setVisible(UWidget *any, bool visible){
        if(any != nullptr){
            //ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

            ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
            // ESlateVisibility::Hidden;
            any->SetVisibility(newStatus);

            //test
            any->SetIsEnabled(visible);

            VISIBLE_FLAG = visible;
        }
    }


    void setVisibleNoCollsion(UWidget *any, bool visible){
        if(any){

            //disbale collsion
            ESlateVisibility visiblity = visible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
            any->SetVisibility(visiblity);
        }
    }

    bool markedVisible(){
        return VISIBLE_FLAG;
    }

private:
    bool TICK_ENABLED = false;
    bool VISIBLE_FLAG = true;
};