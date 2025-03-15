#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/VerticalBox.h"

#include "PresetCornersLayout.generated.h"


class UPlayerUi;

/**
 * preset corners layout with vertical boxes
 */

UCLASS()
class P2_API UPresetCornersLayout : public UcustomUiComponentBase{

    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    void init(UCanvasPanel &parentPanelIn);

    void addChildToTopLeft(UWidget *any);
    void addChildToTopRight(UWidget *any);
    void addChildToBottomLeft(UWidget *any);
    void addChildToBottomRight(UWidget *any);
    void addChildToTopCenter(UWidget *any);

private:
    void createSubLayouts();
    void addChildTo(UVerticalBox *box, UWidget *any);

    UVerticalBox *topLeft = nullptr;
    UVerticalBox *topRight = nullptr;
    UVerticalBox *bottomLeft = nullptr;
    UVerticalBox *bottomRight = nullptr;
    
    UVerticalBox *topCenter = nullptr;

    UCanvasPanel *parentPanel = nullptr;
};