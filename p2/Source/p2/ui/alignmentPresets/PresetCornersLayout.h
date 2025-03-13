#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/VerticalBox.h"

class UPlayerUi;

/**
 * preset corners layout with vertical boxes
 */
class P2_API PresetCornersLayout : public customUiComponentBase{

public:
    PresetCornersLayout();
    PresetCornersLayout(UPlayerUi &parent);
    ~PresetCornersLayout();

    void addChildToTopLeft(UWidget *any);
    void addChildToTopRight(UWidget *any);
    void addChildToBottomLeft(UWidget *any);
    void addChildToBottomRight(UWidget *any);

    
private:
    void resetAllPointers();
    void createSubLayouts();
    void addChildTo(UVerticalBox *box, UWidget *any);

    UVerticalBox *topLeft = nullptr;
    UVerticalBox *topRight = nullptr;
    UVerticalBox *bottomLeft = nullptr;
    UVerticalBox *bottomRight = nullptr;
    


};