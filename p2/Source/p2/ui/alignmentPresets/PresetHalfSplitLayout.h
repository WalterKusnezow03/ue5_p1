#pragma once


#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/HorizontalBox.h"
#include <map>
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"


#include "p2/ui/Widgets/autoContainer/Hbox.h"
#include "p2/ui/Widgets/autoContainer/Vbox.h"

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
        if(baseVertical){
            return baseVertical->baseLayoutPointer();
        }
        return nullptr;
    }
    
    // ---- CLICK DISPATCH AND VISIBILTY ----
    virtual bool dispatchClick() override;
    virtual void setVisible(bool visible) override;

    // --- PURE UNREAL ADDING - NO CLICK DISTPATCHING ---
    void addChildToHeadLine(UWidget *any);

    // --- CUSTOM UI ADDDING - CUSTOM CLICK DISPATCHING ---
    void addChildToHeadLine(UcustomUiComponentBase *any);
    void addChildToLeftVertical(UcustomUiComponentBase *any);
    void addChildToRightVertical(UcustomUiComponentBase *any, int index);


    void showRightSideLayoutAtIndex(int i);

    // bool dispatchClick() override; //MUST COME HERE

private:
    USpacer *createMarginSpacer(int x, int y);
    void createSubLayouts();

    /// @brief TRACKS ALL CHILDS ALSO FOR CLICK LISTENING!
    UVbox *baseVertical = nullptr; //base layout for all sublayouts


    UHbox *rightLeftContainer = nullptr;
    UVbox *rightVertical = nullptr; //deprecated.
    std::map<int, UVbox *> rightPanels; //iterierbar / changable machen
    UVbox *leftVertical = nullptr;
    UHbox *headLineHorizontal = nullptr;

    int currentRightPanelVisible = -1;

    /*DEPRECATED

    UVerticalBox *baseVertical = nullptr; //base layout for all sublayouts
    UHorizontalBox *rightLeftContainer = nullptr;
    UVerticalBox *rightVertical = nullptr; //deprecated.
    std::map<int, UVerticalBox *> rightPanels; //iterierbar / changable machen
    UVerticalBox *leftVertical = nullptr;
    UHorizontalBox *headLineHorizontal = nullptr;*/
};
