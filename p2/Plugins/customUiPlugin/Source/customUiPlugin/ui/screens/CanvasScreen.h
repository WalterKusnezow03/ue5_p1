#pragma once

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "Components/CanvasPanel.h"
#include "Components/BackgroundBlur.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

#include "CanvasScreen.generated.h"

class UPlayerUiBase;

UCLASS()

class CUSTOMUIPLUGIN_API UCanvasScreen : public UcustomUiComponentBase {
    GENERATED_BODY()
public:

    virtual void init(UPlayerUiBase &refin);

    // --- CUSTOM DISPATCHER -> OVERRIDE ---
    virtual bool dispatchClick() override;
    virtual void setVisible(bool visible) override;


    virtual UWidget *baseLayoutPointer() override;
    
    void setBackgroundColor(FLinearColor color);
    void setDefaultBackgroundColor();

    /// @brief ticks all children owned
    /// @param deltatime 
    virtual void Tick(float deltatime) override;

    void AddClickListenedItem(UcustomUiComponentBase *ptr);
    void AddClickListenedItems(TArray<UcustomUiComponentBase *> array);

    /// @brief added child AUTOMATICALLY addet to click and visibilty and Tick DISPATCH!
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

    /// @brief listens for click AND Tick
    TArray<UcustomUiComponentBase *> listenForclickItems;
};