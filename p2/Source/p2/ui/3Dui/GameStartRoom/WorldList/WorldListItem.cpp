#include "WorldListItem.h"

#include "customuipluginbase/Dispatcher/ClickDispatcher.h"

void UWorldListItem::SetText(FString stringin){
    isMarkedFree = false;
    if(UTextBlock *text = TextBlock()){
        text->SetText(FText::FromString(stringin));
    }
}

UTextBlock *UWorldListItem::TextBlock(){
    if(UWidget *raw = GetTextWidget()){
        if(UTextBlock *casted = Cast<UTextBlock>(raw)){
            return casted;
        }
    }
    return nullptr;
}

bool UWorldListItem::ContainsText(FString text){
    return GetText().Contains(text);
}

FString UWorldListItem::GetText(){
    if(UTextBlock *text = TextBlock()){
        return text->GetText().ToString();
    }
    return TEXT("NONE");
}

void UWorldListItem::ClearParent(){
    parent = nullptr;
}

void UWorldListItem::SetParent(UWorldListWidget *parentIn){
    parent = parentIn;
}

void UWorldListItem::NotifyDelete(){
    if(parent){
        parent->NotifyDelete(this);
        SetVisible(false);
        isMarkedFree = true;
    }
}

void UWorldListItem::NotifyLaunch(){
    if(parent){
        parent->NotifyLaunch(this);
    }
}

bool UWorldListItem::IsFree(){
    return isMarkedFree;
}

//override
void UWorldListItem::Tick(float DeltaTime){

}

bool UWorldListItem::dispatchClick(const FVector2D &position){
    ClickDispatcher dispatcher;
    if(markedVisible()){
        if(dispatcher.InBound(GetTextBase(), position)){
            NotifyLaunch();
            return true;
        }
        if(dispatcher.InBound(GetDeleteButton(), position)){
            NotifyDelete();
            return true;
        }
    }
    return false;
}

bool UWorldListItem::dispatchHover(const FVector2D &position){

    return false;
}

void UWorldListItem::SetVisible(bool flag){
    if(flag){
        SetVisibility(ESlateVisibility::Visible);
    }else{
        SetVisibility(ESlateVisibility::Collapsed);
    }
    isMarkedVisible = flag;
}

void UWorldListItem::SetVisibleIfNotFree(bool flag){
    if(!IsFree()){
        SetVisible(flag);
    }
}

bool UWorldListItem::markedVisible(){
    return isMarkedVisible;
}


void UWorldListItem::MarkVisibleBySearch(FString textIn){
    bool flag = ContainsText(textIn);
    SetVisible(flag);
    markedVisibleBySearch = flag;
}
bool UWorldListItem::IsMarkedHiddenBySearch(){
    return !markedVisibleBySearch;
}

