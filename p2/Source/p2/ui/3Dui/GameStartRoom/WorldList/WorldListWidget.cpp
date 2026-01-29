#include "WorldListWidget.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "customuipluginbase/Dispatcher/Filter/WidgetFilter.h"
#include "p2/storageInterface/WorldListSave/WorldListStorageInterface.h"



void UWorldListWidget::SetParent(AGameStartRoom *parentIn){
    parent = parentIn;
    
}

void UWorldListWidget::ResetParent(){
    parent = nullptr;
}

UPanelWidget *UWorldListWidget::GetListAsPanel(){
    if(UWidget *found = GetList()){
        if(UPanelWidget *casted = Cast<UPanelWidget>(found)){
            return casted;
        }
    }
    return nullptr;
}


bool UWorldListWidget::AddNewItem(FString text){
    if(CanAddItem(text)){
        if(UPanelWidget *panel = GetListAsPanel()){
            if(UWorldListItem *made = MakeWorldListItem()){
                made->SetText(text);
                InsertPanelChild(panel, made, 0);
                SaveWorldListToStorage();
                return true;
            }
        }
    }
    return false;
}



bool UWorldListWidget::CanAddItem(FString text){
    TArray<FString> array = GetAllWorldNamesFromWidgets();
    return array.Contains(text) == false;
}

void UWorldListWidget::InsertPanelChild(UPanelWidget* Panel, UWidget* Child, int32 Index)
{
    // Kinder sammeln
    TArray<UWidget*> Tmp;
    Tmp.Reserve(Panel->GetChildrenCount() + 1);

    int32 Num = Panel->GetChildrenCount();
    Index = FMath::Clamp(Index, 0, Num);

    for (int32 i = 0; i < Num; i++)
        Tmp.Add(Panel->GetChildAt(i));

    // altes Panel leeren
    Panel->ClearChildren();

    // neu einfügen
    for (int32 i = 0; i < Tmp.Num(); i++)
    {
        if (i == Index)
            Panel->AddChild(Child);

        Panel->AddChild(Tmp[i]);
    }

    if (Index >= Tmp.Num()){
        Panel->AddChild(Child);
    }
        
}


UWorldListItem *UWorldListWidget::MakeWorldListItem(){

    //find
    if(UWorldListItem *found = FindFreeItem()){
        return found;
    }

    //else: create
    UWorldListItem* created = CreateWidget<UWorldListItem>(GetWorld(), widgetClassForItem);
    if(created){
        created->SetParent(this);
    }
    return created;
}

UWorldListItem *UWorldListWidget::FindFreeItem(){
    WidgetFilter filter;

    UPanelWidget *panel = GetListAsPanel();
    TArray<UWorldListItem *> childs =
        filter.FindDirectChildrenFromPanel<UWorldListItem>(panel);
    if(childs.Num() > 0){
        for (int i = 0; i < childs.Num(); i++){
            if(UWorldListItem *current = childs[i]){
                if(current->IsFree()){
                    panel->RemoveChild(current);
                    return current;
                }
            }
        }
    }
    return nullptr;
}

//dispatch
//ClickDispatcher



void UWorldListWidget::NotifyDelete(UWorldListItem *item){
    if(item){
        RemoveWorldDataFromStorage(item->GetText());
    }
}

void UWorldListWidget::NotifyLaunch(UWorldListItem *item){
    if(item){
        //process
        FString name = item->GetText();
        if(parent){
            parent->LaunchWorld(name);
        }
    }
}





UTextBoxWidget *UWorldListWidget::GetTextBoxSearch(){
    if(UWidget *raw = GetTypeBoxWidget()){
        if(UTextBoxWidget *casted = Cast<UTextBoxWidget>(raw)){
            return casted;
        }
    }
    return nullptr;
}

//override



void UWorldListWidget::Tick(float DeltaTime){

}

/// --- Dispatch Click ---

bool UWorldListWidget::dispatchClick(const FVector2D &position){
    bool resultA = dispatchClickTextbox(position);
    bool resultB = dispatchClickPanel(position);
    if(parent){
        parent->NotifyOnClickDispatch();
    }
    return resultA || resultB;
}

bool UWorldListWidget::dispatchClickTextbox(const FVector2D &position){
    if (UTextBoxWidget *found = GetTextBoxSearch())
    {
        return found->dispatchClick(position);
    }
    return false;
}

bool UWorldListWidget::dispatchClickPanel(const FVector2D &position){
    if(UPanelWidget *panel = GetListAsPanel()){
        ClickDispatcher dispatcher;
        UWorldListItem *casted =
            dispatcher.FindResultFromPanel<UWorldListItem>(panel, position);
        if(casted){
            return casted->dispatchClick(position);
        }
    }
    return false;
}


/// --- Dispatch Hover ---

bool UWorldListWidget::dispatchHover(const FVector2D &position){
    DebugHelper::showScreenMessage("UWorldListWidget::Hover");

    //todo here: dispatch to all items!
    return DispatchHoverToAllItemsForDispatch(position);
}

/// --- Dispatch Keyboard ---

void UWorldListWidget::dispatchUserInput(UserInput &input){
    if(TextBoxIsActive()){
        if(UTextBoxWidget *found = GetTextBoxSearch()){
            found->dispatchUserInput(input);
        }
        NotifyTextChanged();
    }
}



bool UWorldListWidget::TextBoxIsActive(){
    if(UTextBoxWidget *found = GetTextBoxSearch()){
        return found->IsMarkedSelected();
    }
    return false;
}









/// ---- Click trough cards like interface -----
void UWorldListWidget::ShowCardGoRight(){
    ShowCard(currentCardIndex + 1);
}
void UWorldListWidget::ShowCardGoLeft(){
    ShowCard(currentCardIndex - 1);
}


void UWorldListWidget::ShowCard(int index){
    if(IsValidCardIndex(index)){
        currentCardIndex = index;
        int start = currentCardIndex * numItemsPerCard;
        int end = start + numItemsPerCard;

        TArray<UWorldListItem *> items = GetAllItemsFiltered();
        ShowItemsFromTo(start, end, items);
    }
}

bool UWorldListWidget::IsValidCardIndex(int index){
    int cardsTotal = NumItems() / numItemsPerCard;
    return index >= 0 && index < cardsTotal;
}

void UWorldListWidget::ShowItemsFromTo(int start, int end){
    //for all cards
    TArray<UWorldListItem *> items = GetAllItemsFiltered();
    ShowItemsFromTo(start, end, items);
}

void UWorldListWidget::ShowItemsFromTo(
    int start, 
    int end,
    TArray<UWorldListItem *> &items
){
    //for filtered array of cards based on search result
    
    start = std::max(start, 0);
    end = std::min(end, items.Num());
    for (int i = 0; i < items.Num(); i++){
        bool show = i >= start && i < end;
        if(UWorldListItem *current = items[i]){
            current->SetVisibleIfNotFree(show);
        }
    }
}

    

int UWorldListWidget::NumItems(){
    TArray<UWorldListItem *> items = GetAllItemsFiltered();
    return items.Num();
}



TArray<UWorldListItem *> UWorldListWidget::GetAllItemsFiltered(){
    TArray<UWorldListItem *> allChilds = GetAllItems(); //all valid items
    TArray<UWorldListItem *> filtered;
    for (int i = 0; i < allChilds.Num(); i++)
    {
        if(UWorldListItem *current = allChilds[i]){
            //not hidden
            if(!current->IsMarkedHiddenBySearch()){
                filtered.Add(current);
            }
        }
    }

    return filtered;
}


TArray<IBaseUiInterface *> UWorldListWidget::GetAllItemsForDispatch(){
    TArray<IBaseUiInterface *> outArray;
    TArray<UWorldListItem *> allChilds = GetAllItemsFiltered();
    for (int i = 0; i < allChilds.Num(); i++){
        if(UWorldListItem *current = allChilds[i]){
            outArray.Add(current);
        }
    }
    if(UTextBoxWidget *widget = GetTextBoxSearch()){
        outArray.Add(widget);
    }

    DebugHelper::showScreenMessage("UWorldListWidget::Hover Num Items", outArray.Num());

    return outArray;
}

//all items which are not marked free
TArray<UWorldListItem *> UWorldListWidget::GetAllItems(){
    UPanelWidget *panel = GetListAsPanel();
    WidgetFilter filter;
    TArray<UWorldListItem *> allChilds =
        filter.FindDirectChildrenFromPanel<UWorldListItem>(panel);

    TArray<UWorldListItem *> filtered;
    for (int i = 0; i < allChilds.Num(); i++){
        if(UWorldListItem *current = allChilds[i]){
            //still a valid item
            if(!current->IsFree()){
                filtered.Add(current);
            }
        }
    }

    return filtered;
}

void UWorldListWidget::NotifyTextChanged(){
    if(UTextBoxWidget *found = GetTextBoxSearch()){
        FString text = found->GetText();
        
        TArray<UWorldListItem *> items = GetAllItems(); //all valid items
        for (int i = 0; i < items.Num(); i++){
            if(UWorldListItem *current = items[i]){
                current->MarkVisibleBySearch(text);
            }
        }
        ShowCard(0);
    }
}

void UWorldListWidget::OnBeginPlay(){
    LoadWorldListFromStorage();
    ShowCard(0);
}

void UWorldListWidget::OnEndPlay(){
    SaveWorldListToStorage();
}

/// ----- STORAGE INTERFACE ------

TArray<FString> UWorldListWidget::GetAllWorldNamesFromWidgets(){
    TArray<UWorldListItem *> items = GetAllItems(); //all valid items, not filtered
    TArray<FString> outArray;
    for (int i = 0; i < items.Num(); i++){
        if(UWorldListItem *current = items[i]){
            outArray.Add(current->GetText());
        }
    }

    return outArray;
}

/// @brief Loading world list on start
void UWorldListWidget::LoadWorldListFromStorage(){

    DebugHelper::logMessage("UWorldListWidget try load");

    //loads all worlds from storage, otherwise create a new one if debug mode on
    WorldListStorageInterface interface;
    TArray<FString> worldNames;
    if(interface.Load(worldNames)){
        for (int i = 0; i < worldNames.Num(); i++){
            FString current = worldNames[i];
            AddNewItem(current);
            DebugHelper::logMessage(
                FString::Printf(TEXT("UWorldListWidget Loaded World Name From Storage %s"), *current)
            );
        }
    }else{
        AddNewItem(TEXT("None"));
        DebugHelper::logMessage("UWorldListWidget failed to load worlds!");
    }

    DebugHelper::logMessage(FString::Printf(TEXT("UWorldListWidget loaded(%d)"), worldNames.Num()));


}

/// @brief update world list to storage if new added, or removed.
void UWorldListWidget::SaveWorldListToStorage(){
    TArray<FString> worldNames = GetAllWorldNamesFromWidgets();

    WorldListStorageInterface interface;
    interface.Save(worldNames);
}




void UWorldListWidget::RemoveWorldDataFromStorage(FString name){
    WorldListStorageInterface interface;
    interface.DeleteWorld(name);
}