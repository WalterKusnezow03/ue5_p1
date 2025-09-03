#pragma once

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"
#include "CustomUiSlateWidgetBase.generated.h"

/// --- UWIDGET : SLATE to UCUSTOM UI COMPONENT
/// @brief makes the uwidget wrapper for slate base usuable as a UcustomUiComponent for this plugin
/// is first of all a debug class!
UCLASS()
class CUSTOMUIPLUGIN_API UCustomUiSlateWidgetBase : public UcustomUiComponentBase{

    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    virtual void init() override;

    template<typename T>
    void TInit(){
        static_assert(std::is_base_of<UWidgetSlateWrapperBase, T>::value,
            "T must derive from UWidgetSlateWrapperBase!");
        if(WAS_INIT_FLAG){
            return;
        }
        Super::init();
        widget = NewObject<T>(this);
    }



    virtual UWidget *baseLayoutPointer() override {
        return widget;
    }

    ///tick override, propagate to slate child
    virtual void Tick(float deltatime) override;

    /// @brief dispatch click override to ask the Slate widget if the cursor is inside the geometry
    ///method call will come from owning ucustomuicomponent container (canvas, autobox etc.)
    virtual bool dispatchClick() override;


protected:
    //to be overriden
    virtual void MakeWidget();


private:
    UPROPERTY()
    UWidgetSlateWrapperBase *widget = nullptr;


};