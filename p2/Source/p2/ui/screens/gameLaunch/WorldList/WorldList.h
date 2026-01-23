#pragma once

#include "CoreMinimal.h"


#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/GridBox.h"

//#include "p2/ui/screens/gameLaunch/WorldList/item/RemovableTextButton.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "customUiPlugin/ui/Widgets/textBox/TextBoxBase.h"
#include "p2/ui/screens/gameLaunch/WorldList/rtti/WorldButtonPayload.h"
#include "customUiPlugin/slateDerived/TextBased/TextInputSupport/WidgetSlateEditableText.h"

#include "WorldList.generated.h"


class UGameLaunchScreen;

///will create a selectable world list from storage, save it, and launch a game if wanted
///also manages adding new worlds
UCLASS()
class P2_API UWorldList : public UVbox {

    GENERATED_BODY()

public:
    //do not use.
    using UVbox::init;

    //do not use.
    virtual void init() override;

    //use!
    void init(UGameLaunchScreen *launchParent);


    ///@brief adds the world string if not contained yet,
    ///Ui Update: makes a new selector in layout, 
    ///saves additional string to storage
    void AddWorld(FString name);

    ///@brief will remove a world from list and storage completly
    UFUNCTION()
    void removeWorld(UTextButton *item);

    ///@brief tells if a world is already present in the world array
    bool CanAddWorld(FString name);
    

    UFUNCTION()
    void UpdateSearch();

    ///@brief sets width of searchbar -> slate widget setResolution
    void SetWidth(int width);

private:
    //--- scaling ---
    int widthWanted = 100;
    bool widthSetup = false;
    void UpdateScaleIfPossible(IBaseUiInterface *interface, FVector2D resolution);
    //--- scaling ---

    //---button shared rtti (prevents seperate inuse / free list)---
    UPROPERTY()
    UWorldButtonPayload *removeAttribute = nullptr;

    UPROPERTY()
    UWorldButtonPayload *worldNameAttribute = nullptr;

    void UpdatePayLoadWorldButton(UTextButton *worldButton);
    void UpdatePayLoadRemoveButton(UTextButton *removeButton);
    bool IsWorldNameButton(UTextButton *button);
    void UpdateWidthAuto(UTextButton *current);
    void UpdateWidthWorldButton(IBaseUiInterface *interface);
    void UpdateWidthRemoveButton(IBaseUiInterface *interface);
    //---button shared rtti ---

    // parent to call launch world on
    UGameLaunchScreen *gameLaunchScreenParent = nullptr;

    UPROPERTY()
    UGridBox *baseGridBox = nullptr;


    UPROPERTY()
    UWidgetSlateEditableText *searchBar = nullptr;

    //UPROPERTY()
    //UTextBoxBase *searchBar = nullptr;

    UPROPERTY()
    TArray<UTextButton *> textButtonsFree;
    UPROPERTY()
    TArray<UTextButton *> textButtonsInUse;

    TArray<IBaseUiInterface *> makeButtonPair(FString name);
    UTextButton *PopFromFreeList();

    bool isRemoveButton(UTextButton *button);
    bool NameIsValid(FString name);

    TArray<FString> worldNames;

    ///finds all world names at their index in worldnames array which start with a targetname, case ignored
    void filteredWorldNameIndexList(
        FString targetName,
        TArray<int> &hasText,
        TArray<int> &NotHasText
    );

    //called on init to create the base layout
    void createLayout();


    

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
    bool DebugMode = false;
};