#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/CanvasPanel.h"
#include "Components/BackgroundBlur.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

#include "CanvasScreen.generated.h"

class UPlayerUi;

UCLASS()

class P2_API UCanvasScreen : public UcustomUiComponentBase {
    GENERATED_BODY()
public:

    virtual void init(UPlayerUi &refin);

    // --- CUSTOM DISPATCHER -> OVERRIDE ---
    virtual bool dispatchClick() override;
    virtual void setVisible(bool visible) override;


    virtual UWidget *baseLayoutPointer() override;
    
    void setBackgroundColor(FLinearColor color);
    void setDefaultBackgroundColor();


    
    void AddClickListenedItem(UcustomUiComponentBase *ptr);
    void AddClickListenedItems(TArray<UcustomUiComponentBase *> array);

    /// @brief added child AUTOMATICALLY addet to click and visibilty DISPATCH!
    /// @param item 
    void AddChild(UcustomUiComponentBase *item);
    /// @brief 
    /// @param item 
    /// @param offset 
    void AddChild(UcustomUiComponentBase *item, FVector2D offset);

protected:

    UCanvasPanel *baseCanvas = nullptr;

    void createBaseCanvas();
    void updateCanvasScale(UWorld *world);


    UOverlay *baseOverlay = nullptr;
    UBackgroundBlur *backgroundBlur = nullptr;
    UBorder *backgroundColor = nullptr;

    void createBackgroundBlur();
    void createBackgroundBlurAndDefaultColor();

    void createBackgroundOverlay();
    void createColoredBackground();

    TArray<UcustomUiComponentBase *> listenForclickItems;
};