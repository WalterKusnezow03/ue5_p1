#include "UserWidgetBase.h"

#include "DebugPlugin/DebugHelper.h"


void UUserWidgetBase::PostInitProperties()
{
    Super::PostInitProperties();


}


void UUserWidgetBase::Init(){


    //should not happen in constructor. Widget Tree not initialized yet.
    TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
    WidgetTree->RootWidget = TextBlock; // wichtig: setze den TextBlock als RootWidget
    SetLabelText("Some Text");

    DebugHelper::logMessage("UUserWidgetBase:: Construct");

}




void UUserWidgetBase::SetLabelText(FString NewText)
{
    if (TextBlock)
    {
        TextBlock->SetText(FText::FromString(NewText));
    }
}