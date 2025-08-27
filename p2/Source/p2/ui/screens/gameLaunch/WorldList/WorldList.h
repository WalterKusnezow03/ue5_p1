#pragma once

#include "CoreMinimal.h"


#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
#include "p2/ui/screens/gameLaunch/WorldList/item/RemovableTextButton.h"
#include "customUiPlugin/ui/Widgets/textBox/TextBoxBase.h"

#include "WorldList.generated.h"

class UGameLaunchScreen;

///will create a selectable world list from storage, save it, and launch a game if wanted
///also manages adding new worlds
UCLASS()
class P2_API UWorldList : public UcustomUiComponentBase {

    GENERATED_BODY()

public:
    //do not use.
    using UcustomUiComponentBase::init;

    //do not use.
    virtual void init() override;

    //use!
    void init(UGameLaunchScreen *launchParent);

    virtual bool dispatchClick() override;

    /// @brief marks button as invisible: may be needed to not dispatch a click, base layout pointer is 
    /// invisible too!
    /// @param visible
    virtual void setVisible(bool visible) override;

    virtual UWidget *baseLayoutPointer() override{
        if(baseVBox){
            return baseVBox->baseLayoutPointer();
        }
        return nullptr;
    }


    ///@brief adds the world string if not contained yet,
    ///Ui Update: makes a new selector in layout, 
    ///saves additional string to storage
    void AddWorld(FString name);

    ///@brief will remove a world from list and storage completly
    UFUNCTION()
    void removeWorld(URemovableTextButton *item);

    ///@brief tells if a world is already present in the world array
    bool CanAddWorld(FString name);

private:
    UGameLaunchScreen *gameLaunchScreenParent = nullptr;

    UPROPERTY()
    UVbox *baseVBox = nullptr;

    UPROPERTY()
    UTextBoxBase *searchBar = nullptr;

    UPROPERTY()
    TArray<URemovableTextButton *> itemsFree;

    UPROPERTY()
    TArray<URemovableTextButton *> itemsInUse;

    TArray<FString> worldNames;

    //called on init to create the base layout
    void createLayout();


    /// @brief creates a new removable text setup, sets up callbacks, or pops from free list,
    /// but also set upped.
    /// @param name name for world
    URemovableTextButton *makeRemovableTextButton(FString name);

    /// @brief game launch parent to allow world start calls.
    UGameLaunchScreen *gameLaunchParent = nullptr;

    ///@brief creates a new world button and adds it the layout
    void AddWorldInternal(FString name);

    /// @brief Loading world list on start
    void LoadWorldListFromStorage();

    /// @brief update world list to storage if new added, or removed.
    void SaveWorldListToStorage();


    ///@brief will remove a complete world Dir from save path
    void RemoveWorldDataFromStorage(FString worldName);

    //auto create a world string
    bool DebugMode = true;
};