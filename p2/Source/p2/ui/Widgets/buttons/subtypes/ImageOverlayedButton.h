
#pragma once

#include "p2/ui/Widgets/OverlayBased/ImageOverlayed.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/ImageOverlayedButtonBase.h"

#include "ImageOverlayedButton.generated.h"


/**
 * is an button with an text and an image, wraps an imageOverlay class object
 */

/// @brief is a text button with an image, is_A UImageOverlayedButtonBase, Supports textureEnum, not
/// just UTexture2D * pointers, uses asset loader plugin
UCLASS()
class P2_API UImageOverlayedButton : public UImageOverlayedButtonBase {

    GENERATED_BODY()

public:

    //debug
    virtual bool dispatchClick() override {
        if(!markedVisible()){
            return false;
        }
        if(Super::dispatchClick()){
            DebugHelper::showScreenMessage("UImageOverlayedButton pressed");
            return true;
        }
        return false;
    }

    using UImageOverlayedButtonBase::setImage; // <- holt die Basisklassen-Funktion zurück, verdeckung verhindert
    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);

protected:
    //virtual void createImageOverlayedBackground() override;
};