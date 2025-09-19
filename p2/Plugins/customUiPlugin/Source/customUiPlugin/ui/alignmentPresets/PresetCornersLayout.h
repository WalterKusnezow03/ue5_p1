#pragma once

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
#include "PresetCornersLayout.generated.h"

class UCanvasScreen;

/**
 * preset corners layout with vertical boxes
 */

UCLASS()
class CUSTOMUIPLUGIN_API UPresetCornersLayout : public UcustomUiComponentBase{

    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    void init(UCanvasScreen *parentPanelIn);


    void addChildToTopLeft(IBaseUiInterface *any);
    void addChildToTopRight(IBaseUiInterface *any);
    void addChildToBottomLeft(IBaseUiInterface *any);
    void addChildToBottomRight(IBaseUiInterface *any);
    void addChildToTopCenter(IBaseUiInterface *any);

    

private:
    void createSubLayouts();

    void addChildTo(UVbox *box, IBaseUiInterface *any);

    UPROPERTY()
    UVbox *topLeft = nullptr;

    UPROPERTY()
    UVbox *topRight = nullptr;

    UPROPERTY()
    UVbox *bottomLeft = nullptr;

    UPROPERTY()
    UVbox *bottomRight = nullptr;
    
    UPROPERTY()
    UVbox *topCenter = nullptr;

    UPROPERTY()
    UCanvasScreen *parentPanel = nullptr;

    void addToParentPanel(IBaseUiInterface *item, FVector2D anchor, FVector2D alignment);

};