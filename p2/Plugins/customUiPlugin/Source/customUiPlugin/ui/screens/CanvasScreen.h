#pragma once

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/baseInterface/BaseUiInterface.h"
#include "Components/CanvasPanel.h"
#include "Components/BackgroundBlur.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "customUiPlugin/ui/_baseClass/baseParent/UiComponentParent.h"

#include "CanvasScreen.generated.h"

class UPlayerUiBase;


///@brief Base Canvas screen class, use AddChild() method to add any child to the canvas.
UCLASS()
class CUSTOMUIPLUGIN_API UCanvasScreen : public UUiComponentParent {
    GENERATED_BODY()
public:

    virtual bool UsesUserInput(){
        return true;
    }

    //must be called from derived class!
    virtual void init(UPlayerUiBase &refin);

    virtual void CreateLayoutOnInit() {}; //called on init

    virtual UWidget *baseLayoutPointer() override;
    
    void setBackgroundColor(FLinearColor color);
    void setDefaultBackgroundColor();

    /// @brief called from screen stack if a newer screen is closed and this
    /// one is reopended (refresh ui if needed!)
    virtual void OnReOpenScreen() {};

    /// @brief called when the screenstack opens a screen
    virtual void OnOpenScreen() {};

    /// @brief added child AUTOMATICALLY added to click and visibilty and Tick DISPATCH!
    /// @param item 
    void AddChild(IBaseUiInterface *item) override;
    
    /// @brief adds a item at a offset screen position
    /// @param item 
    /// @param offset 
    void AddChild(IBaseUiInterface *item, FVector2D offset);

    /// @brief adds a item, aligned at a screen anchor (0,0) top left, (1,1) bottom right
    /// and aligment of content likewise, gravity / pivot moved in range (0,1)
    /// @param item 
    /// @param screenAnchor screen pos as scalar
    /// @param aligment pivot of item as scalar
    void AddChild(
        IBaseUiInterface *item,
        FVector2D screenAnchor, // corner top left (0,0), bottom right (1,1)
        FVector2D aligment      // gravity / pivot of item (0,0.5), make centered on y
    );

    ///@brief removed from click dispatch and from canvas if stored
    virtual void RemoveChild(IBaseUiInterface *item) override;

    ///@brief removed from canvas AND any IBaseUiInterface having the same widget as ptr.
    virtual void RemoveChild(UWidget *item) override;

    // --- to be REMOVED ---
    //raw adding.
    void AddChild(UWidget *widget);


protected:
    void SetScreenPosition(UWidget *widget, FVector2D &screenPosition);
   

    void createBackgroundBlur();

    /// @brief call if is a ui screen, to have a blurred background darkend.
    void createBackgroundBlurAndDefaultColor();

private: //not protected for a reason!


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

};