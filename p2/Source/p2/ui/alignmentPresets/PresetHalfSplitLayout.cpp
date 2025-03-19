

#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Spacer.h"
#include "p2/ui/PlayerUi.h"
#include "p2/DebugHelper.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"
#include <map>

#include "PresetHalfSplitLayout.h"


void UPresetHalfSplitLayout::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    createSubLayouts();
}



void UPresetHalfSplitLayout::createSubLayouts(){
    baseVertical = NewObject<UVerticalBox>(this);

    headLineHorizontal = NewObject<UHorizontalBox>(this);
    baseVertical->AddChildToVerticalBox(headLineHorizontal);

    USpacer *Spacer = createMarginSpacer(20, 20);
    if (Spacer){
        baseVertical->AddChildToVerticalBox(Spacer);
    }

    rightLeftContainer = NewObject<UHorizontalBox>(this);
    baseVertical->AddChildToVerticalBox(rightLeftContainer);

    leftVertical = NewObject<UVerticalBox>(this);
    rightLeftContainer->AddChildToHorizontalBox(leftVertical);


    USpacer *Spacer1 = createMarginSpacer(20, 20);
    if (Spacer1){
        rightLeftContainer->AddChildToHorizontalBox(Spacer1);
    }

    //rigth side, iterable
    rightVertical = NewObject<UVerticalBox>(this);
    rightLeftContainer->AddChildToHorizontalBox(rightVertical);
    rightPanels[0] = rightVertical; //erstes panel speichern
}


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
        ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
        //ESlateVisibility::Hidden;

        UWidget *second = pair.second;
        if(second != nullptr){
            second->SetVisibility(newStatus);
        }
    }
}


//public api



/*void PresetHalfSplitLayout::addChildToRightVertical(UWidget *any){
    if(rightVertical != nullptr && any != nullptr){
        rightVertical->AddChildToVerticalBox(any);
    }
}*/
void UPresetHalfSplitLayout::addChildToRightVertical(UWidget *any, int index){
    if(any != nullptr){
        //create and add if needed
        if(rightPanels.find(index) == rightPanels.end()){
            rightPanels[index] = NewObject<UVerticalBox>(this);
            rightLeftContainer->AddChildToHorizontalBox(rightPanels[index]);
        }
        //add item.
        UVerticalBox *targetedLayout = rightPanels[index];
        if(targetedLayout != nullptr){
            targetedLayout->AddChildToVerticalBox(any);
        }

        
    }
}







void UPresetHalfSplitLayout::addChildToHeadLine(UWidget *any){
    if(headLineHorizontal != nullptr && any != nullptr){
        headLineHorizontal->AddChildToHorizontalBox(any);
    }
}

void UPresetHalfSplitLayout::addChildToLeftVertical(UWidget *any){
    if(leftVertical != nullptr && any != nullptr){
        leftVertical->AddChildToVerticalBox(any);
    }
}
    
    



void UPresetHalfSplitLayout::addChildToLeftVertical(UcustomUiComponentBase &any){
    addChildToLeftVertical(any.baseLayoutPointer());
}

void UPresetHalfSplitLayout::addChildToRightVertical(UcustomUiComponentBase &any, int index){
    addChildToRightVertical(any.baseLayoutPointer(), index);
}

