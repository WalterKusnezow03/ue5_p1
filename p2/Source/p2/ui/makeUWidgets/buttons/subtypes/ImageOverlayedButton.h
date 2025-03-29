
#pragma once

#include "p2/ui/makeUWidgets/OverlayBased/ImageOverlayed.h"
#include "p2/ui/makeUWidgets/buttons/ButtonBase.h"

#include "ImageOverlayedButton.generated.h"

class UPlayerUi;

/**
 * is an button with an text and an image, wraps an imageOverlay class object
 */

UCLASS()
class P2_API UImageOverlayedButton : public UButtonBase{

    GENERATED_BODY()

public:
    virtual void init() override;

    void setText(FString message);
    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);

private:
    UImageOverlayed *background;

    void createImageOverlayedBackground();

};