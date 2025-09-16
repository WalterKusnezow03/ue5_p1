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
    ///@brief template function to create new UcustomUiComponentBase widget and call init()
    template <typename T>
    static T *NewWidgetInitialized(UObject *parent){
        static_assert(std::is_base_of<UcustomUiComponentBase, T>::value, "T must derive from UcustomUiComponentBase");
        if(parent){
            T* newWidget = NewObject<T>(parent);
            newWidget->init();
            return newWidget;
        }
        return nullptr;
    }

    template<typename T>
    static T* TDuplicateWidgetInitialized(T *toCopy, UObject *parent){
        static_assert(std::is_base_of<UcustomUiComponentBase, T>::value, "T must derive from UcustomUiComponentBase");
        if(toCopy && parent){
            UcustomUiComponentBase *created = toCopy->DuplicateWidgetInitialized(parent);
            if(created){
                T *casted = Cast<T>(created);
                if(casted){
                    return casted;
                }
            }
        }
        return nullptr;
    }

    ///@brief to be overriden!
    virtual UcustomUiComponentBase* DuplicateWidgetInitialized(UObject *parent){
        return nullptr;
    }

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

    bool markedVisible(){
        return VISIBLE_FLAG;
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

   
    

protected:
    bool TICK_ENABLED = true; //true by default!
    bool VISIBLE_FLAG = true;



    ///----- Pay load for buttons for example -------
public:
    /// @brief overriden from interface.
    /// @return 
    UPayLoadBase *GetPayLoad() override{
        return payload;
    }


    ///@brief ownership taken - use getPayload to do funny payload things.
    void SetPayLoad(UPayLoadBase *item){
        //mark garbage not needed here nesecerally.
        payload = item;
    }

protected:
    /// @brief payload item, ptr can be derived.
    UPROPERTY()
    UPayLoadBase *payload = nullptr;
};