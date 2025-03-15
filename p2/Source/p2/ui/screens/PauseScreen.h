#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "CanvasScreen.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/BackgroundBlur.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/TextButton.h"
#include "p2/ui/makeUWidgets/buttons/subtypes/ImageOverlayedButton.h"

#include "Components/VerticalBox.h"

#include "PauseScreen.generated.h"

class UPlayerUi;

UCLASS()
class P2_API UPauseScreen : public UCanvasScreen {

    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    virtual void init(UPlayerUi &playerUiParentref) override;
    
    

private:
    UVerticalBox *menu = nullptr;
    
    UTextButton *exitButton = nullptr;
    UTextButton *loadoutScreenButton = nullptr;

    void createMenu();
    void createButtons();
    void createExitButton();
    void createLoadoutScreenButton();

    void AddChildToMenu(UcustomUiComponentBase &item);
};