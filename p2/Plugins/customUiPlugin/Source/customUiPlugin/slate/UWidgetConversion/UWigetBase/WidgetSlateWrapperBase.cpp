#include "WidgetSlateWrapperBase.h"
#include "customUiPlugin/slate/base/SlateWidgetBase.h"


void UWidgetSlateWrapperBase::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MySlateWidget.Reset(); //sets ptr to nullptr
}

TSharedRef<SWidget> UWidgetSlateWrapperBase::RebuildWidget()
{
    return TRebuildWidget<SSlateWidgetBase>(MySlateWidget);
    
    // MySlateWidget = SNew(SSlateWidgetBase); //hier class name vom slate einfügen
    // return MySlateWidget.ToSharedRef();
}