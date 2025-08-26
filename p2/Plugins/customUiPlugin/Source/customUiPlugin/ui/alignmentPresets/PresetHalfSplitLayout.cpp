

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Spacer.h"
#include "GameCore/DebugHelper.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include <map>

#include "PresetHalfSplitLayout.h"


void UPresetHalfSplitLayout::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    createSubLayouts();
}

/// CLICK DISPATCH AND VISIBLITY
bool UPresetHalfSplitLayout::dispatchClick(){
    if(baseVertical){
        return baseVertical->dispatchClick(); //NOTIFY ALL CHILDREN IN BASE BASE 
    }
    return false;
}

void UPresetHalfSplitLayout::setVisible(bool visible){
    Super::setVisible(visible);
    if(baseVertical){
        baseVertical->setVisible(visible); //NOTIFY ALL CHILDREN
    }
}

void UPresetHalfSplitLayout::Tick(float deltatime){
    if(baseVertical){
        baseVertical->Tick(deltatime); //NOTIFY ALL CHILDREN
    }
}



/// ---- INIT SECTION -----

/// @brief creates all sub layouts!
void UPresetHalfSplitLayout::createSubLayouts(){
    baseVertical = NewObject<UVbox>(this);
    baseVertical->init();

    headLineHorizontal = NewObject<UHbox>(this);
    headLineHorizontal->init();
    baseVertical->AddChild(headLineHorizontal);

    USpacer *Spacer = createMarginSpacer(20, 20);
    if (Spacer){
        baseVertical->AddChild(Spacer);
    }

    rightLeftContainer = NewObject<UHbox>(this);
    rightLeftContainer->init();
    baseVertical->AddChild(rightLeftContainer);

    leftVertical = NewObject<UVbox>(this);
    leftVertical->init();
    rightLeftContainer->AddChild(leftVertical);


    USpacer *Spacer1 = createMarginSpacer(20, 20);
    if (Spacer1){
        rightLeftContainer->AddChild(Spacer1);
    }

    //rigth side, iterable
    rightVertical = NewObject<UVbox>(this);
    rightVertical->init();
    rightLeftContainer->AddChild(rightVertical);
    rightPanels[0] = rightVertical; //erstes panel speichern
}

/// @brief creates a spacer in size x and y
/// @param x 
/// @param y 
/// @return 
USpacer *UPresetHalfSplitLayout::createMarginSpacer(int x, int y){
    // Spacer als "echter" Margin
    USpacer* Spacer = NewObject<USpacer>(this);
    if(Spacer){
        Spacer->SetSize(FVector2D(x, y)); // 20px Abstand
        return Spacer;
    }
    return nullptr;
}

void UPresetHalfSplitLayout::showRightSideLayoutAtIndex(int i){
    for(auto &pair : rightPanels){
        int key = pair.first;
        bool visible = (key == i);
        
        UVbox *second = pair.second;
        if(second){
            second->setVisible(visible); //is propagating listening for clicks to children and
            //hides base layout!
        }

        //save visible index for click dispatch 
        if(visible){
            currentRightPanelVisible = i;
        }

    }
}


//--- PUBLIC API---

/// @brief only use for PURE unreal widgets!
/// @param any 
void UPresetHalfSplitLayout::addChildToHeadLine(UWidget *any){
    if(headLineHorizontal != nullptr && any != nullptr){
        headLineHorizontal->AddChild(any);
    }
}


// ---- UCUSTOM UI ------
void UPresetHalfSplitLayout::addChildToHeadLine(UcustomUiComponentBase *any){
    if(headLineHorizontal != nullptr && any != nullptr){
        headLineHorizontal->AddChild(any);
    }
}

void UPresetHalfSplitLayout::addChildToLeftVertical(UcustomUiComponentBase *any){
    
    if(leftVertical != nullptr && any != nullptr){
        leftVertical->AddChild(any);
    }
}

/// @brief adds an item in the layout at index i and creates a new layout if needed
/// @param any 
/// @param index 
void UPresetHalfSplitLayout::addChildToRightVertical(UcustomUiComponentBase *any, int index){

    if(any){
        if(rightPanels.find(index) == rightPanels.end()){
            rightPanels[index] = NewObject<UVbox>(this);
            rightPanels[index]->init();
            rightLeftContainer->AddChild(rightPanels[index]); //add vertical box to side
        }
        //add item.
        UVbox *targetedLayout = rightPanels[index];
        if(targetedLayout != nullptr){
            targetedLayout->AddChild(any);
        }   
    }
}
