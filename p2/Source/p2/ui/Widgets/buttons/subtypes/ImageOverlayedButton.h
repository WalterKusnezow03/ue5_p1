
#pragma once

#include "p2/ui/Widgets/OverlayBased/ImageOverlayed.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/ImageOverlayedButtonBase.h"

#include "ImageOverlayedButton.generated.h"


/**
 * is an button with an text and an image, wraps an imageOverlay class object
 */

UCLASS()
class P2_API UImageOverlayedButton : public UImageOverlayedButtonBase {

    GENERATED_BODY()

public:

    using UImageOverlayedButtonBase::setImage; // <- holt die Basisklassen-Funktion zurück, verdeckung verhindert
    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);

protected:
    virtual void createImageOverlayedBackground() override;
};