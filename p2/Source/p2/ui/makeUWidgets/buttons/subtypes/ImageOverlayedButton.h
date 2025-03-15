
#pragma once

#include "p2/ui/makeUWidgets/OverlayBased/ImageOverlayed.h"
#include "Components/Button.h"

class UPlayerUi;

/**
 * is an button with an text and an image, wraps an imageOverlay class object
 */
class P2_API ImageOverlayedButton : public ButtonBase{

public:
    ImageOverlayedButton();
    ImageOverlayedButton(UPlayerUi &inref);
    ~ImageOverlayedButton();

    void setText(FString message);
    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);

private:
    ImageOverlayed background;

    void createImageOverlayedBackground();

};