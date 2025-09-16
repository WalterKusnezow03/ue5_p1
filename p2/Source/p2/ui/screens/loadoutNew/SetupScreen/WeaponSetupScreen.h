#pragma once

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
#include "p2/ui/Widgets/buttons/subtypes/ImageOverlayedButton.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"

//debug
#include "customUiPlugin/ui/Widgets/autoContainer/GridBox.h"
#include "customUiPlugin/ui/Widgets/textBox/TextBoxBase.h"

#include "customUiPlugin/ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h"

#include "WeaponSetupScreen.generated.h"


UCLASS()
class P2_API UWeaponSetupScreen : public UCanvasScreen {

    GENERATED_BODY()

public:
    virtual void CreateLayoutOnInit() override;

    /// @brief passed weapon setup helper will be hold by ptr and updated!
    void UpdateScreen(weaponSetupHelper &setup);

    virtual bool dispatchClick() override;

    bool MarkedExit();

protected:
    weaponSetupHelper *referencedSetup = nullptr;

    //Drop down list for each attachment type / weapon selection
    UPROPERTY()
    UVerticalDropDownBase *weaponPicker = nullptr;

    UPROPERTY()
    UVerticalDropDownBase *attachmentSightPicker = nullptr;

    UPROPERTY()
    UVerticalDropDownBase *attachmentGripPicker = nullptr;

    UPROPERTY()
    UVerticalDropDownBase *attachmentMuzzlePicker = nullptr;

   

private:
    bool exitButtonPressed = false;

    void ReversePick(weaponEnum typeWeapon);
    void ReversePick(weaponAttachmentEnum typeAttachment);

    //helper for reverse picking

    ///@brief finds the index from the list, where the Payload of the base
    ///inteface matches the searched enum type, -1 if none found
    int FindIndexFromPayloadData(
        const TArray<IBaseUiInterface *> &listItems,
        weaponAttachmentEnum typeAttachment
    );

    ///@brief finds the index from the list, where the Payload of the base
    ///inteface matches the searched enum type, -1 if none found
    int FindIndexFromPayloadData(
        const TArray<IBaseUiInterface *> &listItems,
        weaponEnum typeAttachment
    );

    //display update.
    void UpdateWeaponAndAttachmentDisplay();

    void CreateTitleBar();

    //weapon picker setup
    void SetupWeaponPicker();

    //attachment picker setup
    void SetupSightPicker();
    void SetupGripPicker();
    void SetupMuzzlePicker();

    //attachment setup quick blueprint 
    UTextButton *MakeWeaponAttachMentButton(weaponAttachmentEnum typeAttachment);

    TArray<UVerticalDropDownBase *> AllLists();
    TArray<UVerticalDropDownBase *> AttachmentLists();

    void HandleClickedItem(IBaseUiInterface *item);



    
};