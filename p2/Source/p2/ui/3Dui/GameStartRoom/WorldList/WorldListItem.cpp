#include "WorldListItem.h"

#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/BorderInterfaceUtil.h"





void UWorldListItem::SetText(FString stringin){
    isMarkedFree = false;
    if(UTextBlock *text = TextBlock()){
        savedText = stringin;
        text->SetText(FText::FromString(AsDisplayText(stringin)));
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
    if(text.Len() <= 0){
        return true;
    }

    return GetText().Contains(text);
}

FString UWorldListItem::GetText(){
    /*if(UTextBlock *text = TextBlock()){
        return text->GetText().ToString();
    }*/
    return savedText;
    //return TEXT("NONE");
}

void UWorldListItem::ClearParent(){
    parent = nullptr;
}

void UWorldListItem::SetParent(UWorldListWidget *parentIn){
    parent = parentIn;
    Init();
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
    
    //to be refactured for border util!
    
    //ClickDispatcher dispatcher;
    if(markedVisible()){
        if(worldItem.dispatchClick(position)){
            NotifyLaunch();
            return true;
        }
        if(deleteItem.dispatchClick(position)){
            NotifyDelete();
            return true;
        }
        
        
        /*
        if(dispatcher.InBound(GetTextBase(), position)){
            NotifyLaunch();
            return true;
        }
        if(dispatcher.InBound(GetDeleteButton(), position)){
            NotifyDelete();
            return true;
        }*/
    }
    return false;
}

bool UWorldListItem::dispatchHover(const FVector2D &position){
    DebugHelper::showScreenMessage("UWorldListItem::HOVER");
    if (markedVisible())
    {
        bool resultA = worldItem.dispatchHover(position);
        bool resultB = deleteItem.dispatchHover(position);
        return resultA || resultB;
    }
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




void UWorldListItem::Init(){
    SetupFromDefaultColors(
        worldItem,
        GetTextBase()
    );
    SetupFromDefaultColors(
        deleteItem,
        GetDeleteButton()
    );

    //unclear.
    MarkVisibleBySearch(TEXT(""));
}



FString UWorldListItem::AsDisplayText(FString s){
    if(s.Len() >= maxDisplayCharacters){
        int dif = s.Len() - maxDisplayCharacters;
        s.LeftChop(dif + 3);
        s += TEXT("...");
    }
    return s;
}