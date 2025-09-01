#include "WorldList.h"

#include "p2/storageInterface/WorldListSave/WorldListStorageInterface.h"
#include "p2/ui/screens/gameLaunch/GameLaunchScreen.h"


void UWorldList::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    createLayout();
    LoadWorldListFromStorage();
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



bool UWorldList::dispatchClick(){
    return baseVbox != nullptr && baseVbox->dispatchClick();
}

void UWorldList::setVisible(bool visible){
    if(baseVbox){
        baseVbox->setVisible(visible);
    }
}


void UWorldList::createLayout(){
    if(!baseVbox){
        baseVbox = NewObject<UVbox>(this); 
        baseVbox->init();
        baseVbox->SetItemsFillHorizontal();
    }

    if(!searchBar && baseVbox){
        searchBar = NewObject<UTextBoxBase>(this);
        searchBar->init();
        searchBar->SetHintText("Search world...");
        baseVbox->AddChild(searchBar);

        //make search update delegate
        FSimpleDelegate updateTextDelegate = FSimpleDelegate::CreateUObject(this, &UWorldList::UpdateSearch);
        searchBar->SetCallBackTextUpdate(updateTextDelegate);

        //FSimpleDelegate::CreateUObject(uclassInstance*, &<classname>::<methodname>)
        /*FSimpleDelegate::CreateLambda([uclassInstance*, SomeValue]()
        {
            uclassInstance->SomeUFunction(SomeValue);
        }*/
    }

    

    
    
    if(!baseGridBox && baseVbox){
        baseGridBox = NewObject<UGridBox>(this);
        baseGridBox->init(1,2); //2 columns. (world button and delete button)
        baseVbox->AddChild(baseGridBox);
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
        
        TArray<UcustomUiComponentBase *> newRow = makeButtonPair(name);
        baseGridBox->AddRow(newRow);
        //AddRow(TArray<UcustomUiComponentBase *> &items)
    }
}


void UWorldList::removeWorld(UTextButton *item){
    if(!item){
        return;
    }

    //remove from string list
    FString worldName = item->getText();
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
        TArray<UcustomUiComponentBase *> removed = baseGridBox->RemovedItemsFromRemoveRow(item);
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




TArray<UcustomUiComponentBase*> UWorldList::makeButtonPair(FString name){
    UTextButton *worldButton = PopFromFreeList();
    UTextButton *removeButton = PopFromFreeList();

    if(worldButton){
        worldButton->setText(name);

        //start world Callback
        if(gameLaunchScreenParent){
            FSimpleDelegate startWorldDelegate = FSimpleDelegate::CreateLambda([this, worldButton]()
                {
                    FString worldName = worldButton->getText();
                    this->gameLaunchScreenParent->launchWorld(worldName);
                }
            );
            worldButton->SetCallBack(startWorldDelegate);
        }

        if(removeButton){
            removeButton->setText("Delete");
    
            //remove callback, push world button for name!
            FSimpleDelegate removeDelegate = FSimpleDelegate::CreateLambda([this, worldButton]()
                {
                    this->removeWorld(worldButton);
                }
            );
            removeButton->SetCallBack(removeDelegate);
        }
    }

    TArray<UcustomUiComponentBase *> outArray;
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
        Ptr = NewObject<UTextButton>(this);
        Ptr->init();
    }
    textButtonsInUse.Add(Ptr);
    return Ptr;
}

bool UWorldList::isRemoveButton(UTextButton *button){
    if(button){
        return button->getText() == "Delete";
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
        if(worldNames.Num() > 0){
            if(DebugMode){
                AddWorld(TEXT("AdditionalDebugString"));
                return;
            }
        }


       
    }


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