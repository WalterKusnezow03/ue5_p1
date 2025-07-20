#pragma once

#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "customUiPlugin/ui/Widgets/OverlayBased/ImageOverlayedBase.h"


#include "ImageOverlayed.generated.h"


/**
 * will display an image in background and a text at the top
 */

UCLASS()
class P2_API UImageOverlayed : public UImageOverlayedBase {

    GENERATED_BODY()

public:
    
    using UImageOverlayedBase::setImage; // <- holt die Basisklassen-Funktion zurück, verdeckung verhindert
    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);

};
