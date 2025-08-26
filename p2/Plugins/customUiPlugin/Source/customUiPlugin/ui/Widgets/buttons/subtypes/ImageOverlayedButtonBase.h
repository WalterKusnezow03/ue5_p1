
#pragma once

#include "customUiPlugin/ui/Widgets/OverlayBased/ImageOverlayedBase.h"
#include "customUiPlugin/ui/Widgets/buttons/ButtonBase.h"

#include "ImageOverlayedButtonBase.generated.h"


/**
 * is an button with an text and an image, wraps an imageOverlay class object
 */

UCLASS()
class CUSTOMUIPLUGIN_API UImageOverlayedButtonBase : public UButtonBase{

    GENERATED_BODY()

public:
    virtual void init() override;

    void setImage(UTexture2D* type);
    void setImage(UTexture2D* type, FVector2D scale);

    void setText(FString message);

protected:
    UPROPERTY()
    UImageOverlayedBase *background;

    virtual void createImageOverlayedBackground();

    void addBackgroundToBase();
};