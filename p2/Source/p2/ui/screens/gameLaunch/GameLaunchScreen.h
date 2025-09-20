#pragma once


#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "p2/ui/Widgets/ImageBased/WidgetImageExtended.h"

#include "GameLaunchScreen.generated.h"

class UPlayerUiBase;
class UHbox;
class UVbox;
class UTextBoxBase;
class UWorldList;
class UWidgetSlateEditableText;

/**
 * loadout screen for the player to choose weapons and attachments
 * 
 */
UCLASS()
class P2_API UGameLaunchScreen : public UCanvasScreen{
    GENERATED_BODY()

public:
    
    virtual void init(UPlayerUiBase &ref);

    virtual bool dispatchClick() override;

    /// @brief launch a world from UWorldList
    /// @param world 
    UFUNCTION()
    void launchWorld(FString world);

    /// @brief launch a world from creation mode, adds to world list and saves to storage
    /// @param world 
    UFUNCTION()
    void createAndLaunchWorldFromTypeField();




protected:


private:
    int WidthOfLayout = 300;
    int HeightTopBar = 40;

    //title hbox
    UPROPERTY()
    UHbox *topHbox = nullptr;


    //layout for type new world, create button
    UPROPERTY()
    UHbox *createBarHbox = nullptr;

    //UPROPERTY()
    //UTextBoxBase *typeFieldWorld = nullptr;
    UPROPERTY()
    UWidgetSlateEditableText *typeFieldWorld = nullptr;
    

    UPROPERTY()
    UWidgetImageExtended *typeFieldConfirmButton = nullptr;

    //menu vbox (store bar and list)
    UPROPERTY()
    UVbox *menuVbox = nullptr; //UVbox

    UPROPERTY()
    UWorldList *worldListPicker = nullptr;

    //screen name bar
    void createTopBar();

    //menu
    void createMenu();
    void createTypeFieldForWorldCreation();
    void createWorldList();

    //debug
    void AddDebugElements();
};