#pragma once

#include "Components/Widget.h"
#include "WidgetSlateWrapperBase.generated.h"

class SSlateWidgetBase;

/// --- SLATE TO UWIDGET WRAPPER---
/// @brief makes the slate base usuable as a UWidget for UMG
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetSlateWrapperBase : public UWidget
{
    GENERATED_BODY()

public:
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;


    //rebuild function made template
    template <typename TSlateWidgetType>
    TSharedRef<TSlateWidgetType> TRebuildWidget(TSharedPtr<TSlateWidgetType> &sharedPtr){
        sharedPtr = SNew(TSlateWidgetType); //hier class name vom slate einfügen
        return sharedPtr.ToSharedRef();
    }

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

private:
    TSharedPtr<SSlateWidgetBase> MySlateWidget;
};