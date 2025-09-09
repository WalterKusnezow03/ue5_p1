#pragma once


#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"

#include <map>
#include "Components/Spacer.h"



#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"

#include "PresetHalfSplitLayout.generated.h"


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
class CUSTOMUIPLUGIN_API UPresetHalfSplitLayout : public UcustomUiComponentBase{
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
    virtual void SetVisible(bool visible) override;
    virtual void Tick(float deltatime) override;

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

    /// @brief TRACKS ALL CHILDS ALSO FOR CLICK LISTENING AND TICK!!
    UPROPERTY()
    UVbox *baseVertical = nullptr; //base layout for all sublayouts

    //INNER CONTAINERS DO NOT TICK! AGREGATION NOT OWNERSHIP
    UPROPERTY()
    UHbox *rightLeftContainer = nullptr;

    UPROPERTY()
    UVbox *rightVertical = nullptr;
    std::map<int, UVbox *> rightPanels; //iterierbar / changable machen
    
    UPROPERTY()
    UVbox *leftVertical = nullptr;

    UPROPERTY()
    UHbox *headLineHorizontal = nullptr;

    int currentRightPanelVisible = -1;

};
