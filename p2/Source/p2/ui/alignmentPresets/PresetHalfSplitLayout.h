#pragma once


#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/HorizontalBox.h"
#include <map>
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"

#include "PresetHalfSplitLayout.generated.h"

class UPlayerUi;

/**
 * screen for the player to choose weapons and attachments for example, layout A and B side to side
 * 
 * headline 
 * -------- | ---------------------
 * weapons  | weapon or attachment 
 * - picked | picker (iterable(?))
 *   ONE    |     MUTLIPLE
 *          |
 */

UCLASS()
class P2_API UPresetHalfSplitLayout : public UcustomUiComponentBase{
    GENERATED_BODY()

public:
    virtual void init() override;

    virtual UWidget *baseLayoutPointer() override {
        return baseVertical;
    }

    void addChildToHeadLine(UWidget *any);
    //void addChildToRightVertical(UWidget *any);
    void addChildToLeftVertical(UWidget *any);
    void addChildToRightVertical(UWidget *any, int index);

    void addChildToLeftVertical(UcustomUiComponentBase &any);
    void addChildToRightVertical(UcustomUiComponentBase &any, int index);


    void showRightSideLayoutAtIndex(int i);

private:
    USpacer *createMarginSpacer(int x, int y);
    void createSubLayouts();

    UVerticalBox *baseVertical = nullptr; //base layout for all sublayouts

    UHorizontalBox *rightLeftContainer = nullptr;

    UVerticalBox *rightVertical = nullptr; //deprecated.
    
    std::map<int, UVerticalBox *> rightPanels; //iterierbar / changable machen

    UVerticalBox *leftVertical = nullptr;
    UHorizontalBox *headLineHorizontal = nullptr;
};
