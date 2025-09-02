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
    virtual void init() override {
        if(WAS_INIT_FLAG){
            return;
        }
        Super::init();
        MakeWidget();
        MakeBaseLayout();
    };

    virtual UWidget *baseLayoutPointer() override {
        return baseLayout;
    }

    ///tick override, propagate to slate child
    virtual void Tick(float deltatime) override;

    /// @brief dispatch click override to ask the Slate widget if the cursor is inside the geometry
    ///method call will come from owning ucustomuicomponent container (canvas, autobox etc.)
    virtual bool dispatchClick() override;

    ///@brief sets the umg size of the widget, for example 200 x 200
    void SetWidgetScale(FVector2D scalePixels);

protected:
    //to be overriden
    virtual void MakeWidget();

    //call AFTER "MakeWidget()" function!
    void MakeBaseLayout();

private:
    UPROPERTY()
    UWidgetSlateWrapperBase *widget = nullptr;

    /// @brief widget must be wrapped in SizeBox, otherwise its broken.s
    UPROPERTY()
    USizeBox *baseLayout = nullptr;

};