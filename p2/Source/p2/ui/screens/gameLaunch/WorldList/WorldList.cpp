#include "WorldList.h"

#include "p2/storageInterface/WorldListSave/WorldListStorageInterface.h"
#include "p2/ui/screens/gameLaunch/WorldList/rtti/WorldButtonPayload.h"
#include "p2/ui/screens/gameLaunch/GameLaunchScreen.h"


void UWorldList::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createLayout();
    LoadWorldListFromStorage();
    DebugHelper::logMessage("UWorldList init");
}

void UWorldList::init(UGameLaunchScreen *launcher){
    if(WAS_INIT_FLAG){
        return;
    }
    if(launcher){
        gameLaunchScreenParent = launcher;
        init();
    }
}





void UWorldList::createLayout(){
    
    if(!searchBar){
        searchBar = NewObject<UWidgetSlateEditableText>(this);
        searchBar->SetHintText("Search world...");
        AddChild((IBaseUiInterface*) searchBar);

        //make search update delegate
        FSimpleDelegate updateTextDelegate = FSimpleDelegate::CreateUObject(this, &UWorldList::UpdateSearch);
        searchBar->SetCallBackTextUpdate(updateTextDelegate);

        //FSimpleDelegate::CreateUObject(uclassInstance*, &<classname>::<methodname>)
        /*FSimpleDelegate::CreateLambda([uclassInstance*, SomeValue]()
        {
            uclassInstance->SomeUFunction(SomeValue);
        }*/

        CreateSpacer(10);
    }

    if(!baseGridBox){
        baseGridBox = NewObject<UGridBox>(this);
        baseGridBox->init(1,2); //2 columns. (world button and delete button)
        AddChild(baseGridBox);
    }
}

bool UWorldList::CanAddWorld(FString name){
    return !worldNames.Contains(name);
}

bool UWorldList::NameIsValid(FString name){
    return name != TEXT("Delete");
}



///@brief external add world, saves world name, if not contained yet!
void UWorldList::AddWorld(FString name){
    if(CanAddWorld(name)){
        
        AddWorldInternal(name);
        worldNames.Add(name);
        SaveWorldListToStorage();

        DebugHelper::logMessage("UWorldList: added ", name);
    }
}

/// @brief intern Add World, NOT SAVED, just for button creation
/// @param name 
void UWorldList::AddWorldInternal(FString name){
    if(baseGridBox){
        
        TArray<IBaseUiInterface *> newRow = makeButtonPair(name);
        baseGridBox->AddRow(newRow);
    }
}


void UWorldList::removeWorld(UTextButton *item){
    if(!item){
        return;
    }

    //remove from string list
    FString worldName = item->GetText();
    bool wasFound = false;
    if (worldNames.Num() > 0)
    {
        int32 IndexName;
        if (worldNames.Find(worldName, IndexName)){
            worldNames[IndexName] = worldNames[worldNames.Num() - 1];
            worldNames.Pop();
            wasFound = true;
        }
    }

    //remove row from grid and free all items
    if(baseGridBox){
        TArray<IBaseUiInterface *> removed = baseGridBox->RemovedItemsFromRemoveRow(item);
        for (int i = 0; i < removed.Num(); i++){
            if(removed[i]){
                UTextButton *casted = Cast<UTextButton>(removed[i]);
                if(casted){
                    textButtonsFree.Add(casted);
                }

                //remove from use array
                int32 Index;
                if (textButtonsInUse.Find(casted, Index)){
                    textButtonsInUse[Index] = textButtonsInUse[textButtonsInUse.Num() - 1];
                    textButtonsInUse.Pop();
                }
            }
        }
    }

    //save changes made
    SaveWorldListToStorage();
    if(wasFound){
        RemoveWorldDataFromStorage(worldName);
    }
}




TArray<IBaseUiInterface*> UWorldList::makeButtonPair(FString name){
    UTextButton *worldButton = PopFromFreeList();
    UTextButton *removeButton = PopFromFreeList();

    if(worldButton){
        worldButton->SetText(name);

        //start world Callback
        if(gameLaunchScreenParent){
            FSimpleDelegate startWorldDelegate = FSimpleDelegate::CreateLambda([this, worldButton]()
                {
                    FString worldName = worldButton->GetText();
                    this->gameLaunchScreenParent->launchWorld(worldName);
                    DebugHelper::logMessage("Try launch world!");
                }
            );
            worldButton->SetCallBack(startWorldDelegate);
        }

        if(removeButton){
            removeButton->SetText("Delete");
    
            //remove callback, push world button for name!
            FSimpleDelegate removeDelegate = FSimpleDelegate::CreateLambda([this, worldButton]()
                {
                    this->removeWorld(worldButton);
                }
            );
            removeButton->SetCallBack(removeDelegate);
        
        }
    }

    //update scale
    UpdateWidthWorldButton(worldButton);
    UpdateWidthRemoveButton(removeButton);
    UpdatePayLoadWorldButton(worldButton);
    UpdatePayLoadRemoveButton(removeButton);

    TArray<IBaseUiInterface *> outArray;
    if(worldButton)
        outArray.Add(worldButton);

    if(removeButton)
        outArray.Add(removeButton);

    return outArray;
}

UTextButton *UWorldList::PopFromFreeList(){
    UTextButton *Ptr = nullptr;
    if(textButtonsFree.Num() > 0){
        Ptr = textButtonsFree[textButtonsFree.Num() - 1];
        textButtonsFree.Pop();
    }else{
        Ptr = NewWidgetInitialized<UTextButton>(this);
    }
    textButtonsInUse.Add(Ptr);
    return Ptr;
}

bool UWorldList::isRemoveButton(UTextButton *button){
    if(button){
        return button->GetText() == "Delete";
    }
    return false;
}

/// --- SEARCH INTERFACE ---
void UWorldList::UpdateSearch(){
    if(!searchBar){
        return;
    }
    if(!baseGridBox){
        return;
    }

    FString name = searchBar->GetText();
    if (name.Len() <= 0)
    {
        //show all
        baseGridBox->SetAllRowsVisible(true);
    }
    else
    {
        //filter list

        //filter world list strings
        TArray<int> hasText;
        TArray<int> NotHasText;
        filteredWorldNameIndexList(name, hasText, NotHasText);

        //show rows
        for (int i = 0; i < hasText.Num(); i++){
            int index = hasText[i];
            FString worldNameCompare = worldNames[index];

            for (int buttonIndex = 0; buttonIndex < textButtonsInUse.Num(); buttonIndex++){
                UTextButton *button = textButtonsInUse[buttonIndex];
                if(!isRemoveButton(button)){
                    //show row if text same
                    if(button->CompareText(worldNameCompare)){
                        baseGridBox->SetRowVisible(button, true);
                    }
                }
            }
        }

        //hide rows
        for (int i = 0; i < NotHasText.Num(); i++){
            int index = NotHasText[i];
            FString worldNameCompare = worldNames[index];

            for (int buttonIndex = 0; buttonIndex < textButtonsInUse.Num(); buttonIndex++){
                UTextButton *button = textButtonsInUse[buttonIndex];
                if(!isRemoveButton(button)){
                    //show row if text same
                    if(button->CompareText(worldNameCompare)){
                        baseGridBox->SetRowVisible(button, false);
                    }
                }
            }
        }

    }
}


void UWorldList::filteredWorldNameIndexList(
    FString targetName,
    TArray<int> &hasText,
    TArray<int> &NotHasText
){
    for (int i = 0; i < worldNames.Num(); i++){
        FString &current = worldNames[i];
        
        if (current.StartsWith(targetName, ESearchCase::IgnoreCase)){
            hasText.Add(i);
        }else{
            NotHasText.Add(i);
        }
    }
}


/// ----- STORAGE INTERFACE ------
/// @brief Loading world list on start
void UWorldList::LoadWorldListFromStorage(){

    DebugHelper::logMessage("UWorldList try load");

    //loads all worlds from storage, otherwise create a new one if debug mode on
    WorldListStorageInterface interface;
    if(interface.Load(worldNames)){
        for (int i = 0; i < worldNames.Num(); i++){
            FString current = worldNames[i];
            AddWorldInternal(current);
            DebugHelper::logMessage(
                FString::Printf(TEXT("UWorldList Loaded World Name From Storage %s"), *current)
            );
        }

        //add a debug string to see a world was loaded.
        if(worldNames.Num() == 0){
            if(DebugMode){
                AddWorld(TEXT("AdditionalDebugString"));
                return;
            }
        }
    }else{
        AddWorld(TEXT("None"));
        DebugHelper::logMessage("UWorldlist failed to load worlds!");
    }

    DebugHelper::logMessage(FString::Printf(TEXT("UWorldlist loaded(%d)"), worldNames.Num()));


    //add a string if no world was loaded for visibility.
    if(DebugMode){
        AddWorld(TEXT("NoneFoundDebugWorld"));
    }
    
}

/// @brief update world list to storage if new added, or removed.
void UWorldList::SaveWorldListToStorage(){
    WorldListStorageInterface interface;
    interface.Save(worldNames);
}



void UWorldList::RemoveWorldDataFromStorage(FString name){
    WorldListStorageInterface interface;
    interface.DeleteWorld(name);
}







/// ----- SCALING -------
void UWorldList::SetWidth(int width){
    //set with of search bar fixed
    UpdateScaleIfPossible(searchBar, FVector2D(width, 30));
    widthWanted = width;
    widthSetup = true;

    for (int i = 0; i < textButtonsInUse.Num(); i++){
        UTextButton *current = textButtonsInUse[i];
        if(current){
            UpdateWidthAuto(current);
        }
    }
}

void UWorldList::UpdateWidthAuto(UTextButton *current){
    if(current){
        if(IsWorldNameButton(current)){
            UpdateWidthWorldButton(current);
        }else{
            UpdateWidthRemoveButton(current);
        }
    }
}

void UWorldList::UpdateWidthWorldButton(IBaseUiInterface *interface){
    if(widthSetup){
        int widthWorldText = widthWanted * 0.6f;
        UpdateScaleIfPossible(interface, FVector2D(widthWorldText, 30));
    }
}
void UWorldList::UpdateWidthRemoveButton(IBaseUiInterface *interface){
    if(widthSetup){
        int widthWorldText = widthWanted * 0.6f;
        int widthDeleteText = widthWanted - widthWorldText;
        UpdateScaleIfPossible(interface, FVector2D(widthDeleteText, 30));
    }
}

void UWorldList::UpdateScaleIfPossible(IBaseUiInterface *interface, FVector2D resolution){
    if(interface){
        //try to find slate widget
        UWidgetSlateWrapperBase *casted = Cast<UWidgetSlateWrapperBase>(interface);
        if(!casted){
            UWidget *base = interface->baseLayoutPointer();
            if(base){
                casted = Cast<UWidgetSlateWrapperBase>(base);
            }
        }
        if(casted){
            casted->SetResolution(resolution);
        }
    }
}



// -- FAKE RTTI --
void UWorldList::UpdatePayLoadWorldButton(UTextButton *worldButton){
    if(worldButton){
        if(!worldNameAttribute){
            worldNameAttribute = NewObject<UWorldButtonPayload>();
            worldNameAttribute->SetTypeWorldNameButton();
        }
        worldButton->SetPayLoad(worldNameAttribute);
    }
}
void UWorldList::UpdatePayLoadRemoveButton(UTextButton *removeButton){
    if(removeButton){
        if(!removeAttribute){
            removeAttribute = NewObject<UWorldButtonPayload>();
            removeAttribute->SetTypeRemoveButton();
        }
        removeButton->SetPayLoad(removeAttribute);
    }
}

bool UWorldList::IsWorldNameButton(UTextButton *button){
    if(button){
        UPayLoadBase *payload = button->GetPayLoad();
        if(payload){
            UWorldButtonPayload *casted = Cast<UWorldButtonPayload>(payload);
            if(casted){
                return casted->IsWorldNameButton();
            }
        }
    }
    return false;
}
