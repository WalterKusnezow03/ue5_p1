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
    /// @brief adds a item at a offset screen position
    /// @param item 
    /// @param offset 
    void AddChild(UcustomUiComponentBase *item, FVector2D offset);

    /// @brief adds a item, aligned at a screen anchor (0,0) top left, (1,1) bottom right
    /// and aligment of content likewise, gravity / pivot moved in range (0,1)
    /// @param item 
    /// @param screenAnchor screen pos as scalar
    /// @param aligment pivot of item as scalar
    void AddChild(
        UcustomUiComponentBase *item,
        FVector2D screenAnchor, // corner top left (0,0), bottom right (1,1)
        FVector2D aligment      // gravity / pivot of item (0,0.5), make centered on y
    );

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