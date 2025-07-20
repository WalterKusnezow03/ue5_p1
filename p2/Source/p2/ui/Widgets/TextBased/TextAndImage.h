#pragma once

#include "AssetPlugin/gamestart/assetEnums/textureEnum.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/Widgets/TextBased/TextAndImageBase.h"


#include "TextAndImage.generated.h"


/**
 * label with image like class in an horizontal layout
 */

UCLASS()
class P2_API UTextAndImage : public UTextAndImageBase{
    GENERATED_BODY()

public:
    
    using UTextAndImageBase::setImage; // <- holt die Basisklassen-Funktion zurück, verdeckung verhindert
    void setImage(textureEnum type);
    void setImage(textureEnum type, FVector2D scale);
    void setImage(FString path);

};