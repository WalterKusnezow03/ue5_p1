#pragma once

#include "Components/Widget.h"
#include "customUiPlugin/baseInterface/WidgetHelper.h"
#include "customUiPlugin/baseInterface/BaseUiInterface.h"
#include "customUiComponentBase.generated.h"

class UPlayerUiBase;

/**
 * base functionality each custom ui element should have 
 * (the pointer to the main owning UPlayerUi instance)
 */
UCLASS()
class CUSTOMUIPLUGIN_API UcustomUiComponentBase : public UObject, public IBaseUiInterface{

    GENERATED_BODY()

public:
    // --- CALL ON ANY DERIVED BEFORE USING! ---
    //after construct:
    virtual void init(){
        WAS_INIT_FLAG = true;
    }

    //does not need to be public by default, made public if needed
protected:    
    virtual void init(UPlayerUiBase &refin){
        saveParent(refin);
        WAS_INIT_FLAG = true;
    }

public:


    // --- manual click dispatch, must be overriden if has childs!---
    //OVERRIDE THIS METHOD!
    virtual bool dispatchClick() override {
        return false;
    }

    /// @brief marks self as invisible: may be needed to not dispatch a click, base layout pointer is 
    /// invisible too! - OVERRIDE THIS METHOD!
    /// @param visible 
    virtual void SetVisible(bool visible) override {
        //setVisible(baseLayoutPointer(), visible);
        
        WidgetHelper::SetVisible(baseLayoutPointer(), visible);
        VISIBLE_FLAG = visible;
    }

    //--- Ticker section ---
    //destruct
    virtual void BeginDestroy() override
    {
        //remove tick from tick handler
        TICK_ENABLED = false;

        Super::BeginDestroy();
    }

    ///@brief is called by tick handler if subscribed
    virtual void Tick(float DeltaTime) override {
        if(!TICK_ENABLED){
            return;
        }
    }

    //--- Ticker section end ---


    void saveParent(UPlayerUiBase &ref){
        playerUiParent = &ref;
    }

    //MUST BE OVERRIDEN!!!
    virtual UWidget *baseLayoutPointer() override {
        return nullptr;
    }


protected:
    UPlayerUiBase *playerUiParent = nullptr;

    bool WAS_INIT_FLAG = false;



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

protected:
    bool TICK_ENABLED = true; //true by default!
private:
    bool VISIBLE_FLAG = true;
};