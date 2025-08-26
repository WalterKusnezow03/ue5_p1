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


///@brief Base Canvas screen class, use AddChild() method to add any child to the canvas.
UCLASS()
class CUSTOMUIPLUGIN_API UCanvasScreen : public UcustomUiComponentBase {
    GENERATED_BODY()
public:

    //must be called from derived class!
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


    /// @brief added child AUTOMATICALLY added to click and visibilty and Tick DISPATCH!
    /// @param item 
    void AddChild(UcustomUiComponentBase *item);
    /// @brief 
    /// @param item 
    /// @param offset 
    void AddChild(UcustomUiComponentBase *item, FVector2D offset);

protected:
    void createBackgroundBlur();
    void createBackgroundBlurAndDefaultColor();

private: //not protected for a reason!

    void AddClickListenedItem(UcustomUiComponentBase *ptr);
    void AddClickListenedItems(TArray<UcustomUiComponentBase *> array);

    UPROPERTY()
    UCanvasPanel *baseCanvas = nullptr;

    void createBaseCanvas();
    void updateCanvasScale(UWorld *world);

    UPROPERTY()
    UOverlay *baseOverlay = nullptr;

    UPROPERTY()
    UBackgroundBlur *backgroundBlur = nullptr;

    UPROPERTY()
    UBorder *backgroundColor = nullptr;

    

    void createBackgroundOverlay();
    void createColoredBackground();

    /// @brief listens for click AND Tick
    TArray<UcustomUiComponentBase *> listenForclickItems;
};