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
    return baseVBox != nullptr && baseVBox->dispatchClick();
}

void UWorldList::setVisible(bool visible){
    if(baseVBox){
        baseVBox->setVisible(visible);
    }
}


void UWorldList::createLayout(){
    if(!baseVBox){
        baseVBox = NewObject<UVbox>(this);
        baseVBox->init();
    }
}


///@brief external add world, saves world name, if not contained yet!
void UWorldList::AddWorld(FString name){
    if(!worldNames.Contains(name)){
        AddWorldInternal(name);
        worldNames.Add(name);
        SaveWorldListToStorage();

        DebugHelper::logMessage("UWorldList: added ", name);
    }
}

/// @brief intern Add World, NOT SAVED, just for button creation
/// @param name 
void UWorldList::AddWorldInternal(FString name){
    if(baseVBox){
        URemovableTextButton *button = makeRemovableTextButton(name);
        itemsInUse.Add(button);
        baseVBox->AddChild(button);
    }
}

void UWorldList::removeWorld(URemovableTextButton *item){
    if(!item){
        return;
    }

    //remove from string list
    FString worldName = item->GetText();
    if (worldNames.Num() > 0)
    {
        int32 IndexName;
        if (worldNames.Find(worldName, IndexName)){
            worldNames[IndexName] = worldNames[worldNames.Num() - 1];
            worldNames.Pop();
        }
    }

    //remove from in use item pointer list
    if(baseVBox){
        baseVBox->RemoveChild(item);

        //remove from use array
        int32 Index;
        if (itemsInUse.Find(item, Index)){
            itemsInUse[Index] = itemsInUse[itemsInUse.Num() - 1];
            itemsInUse.Pop();
        }
        itemsFree.Add(item);
    }

    //save changes made
    SaveWorldListToStorage();
}

URemovableTextButton *UWorldList::makeRemovableTextButton(FString name){
    URemovableTextButton *Ptr = nullptr;
    if(itemsFree.Num() > 0){
        Ptr = itemsFree[itemsFree.Num() - 1];
        itemsFree.Pop();
    }else{
        Ptr = NewObject<URemovableTextButton>(this);
        Ptr->init();
    }

    if(Ptr){
        Ptr->SetText(name);

        //FSimpleDelegate::CreateUObject(uclassInstance*, &<classname>::<methodname>)
        /*FSimpleDelegate::CreateLambda([uclassInstance*, SomeValue]()
        {
            uclassInstance->SomeUFunction(SomeValue);
        }*/

        //update callback: Click and Launch world!

        //remove callback
        FSimpleDelegate removeDelegate = FSimpleDelegate::CreateLambda([this, Ptr]()
            {
                this->removeWorld(Ptr);
            }
        );
        Ptr->SetRemoveCallback(removeDelegate);
        

        //start world Callback
        if(gameLaunchScreenParent){
            FSimpleDelegate startWorldDelegate = FSimpleDelegate::CreateLambda([this, Ptr]()
                {
                    FString worldName = Ptr->GetText();
                    this->gameLaunchScreenParent->launchWorld(worldName);
                }
            );
            Ptr->SetTextButtonCallBack(startWorldDelegate);
        }
        


    }
    return Ptr;
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