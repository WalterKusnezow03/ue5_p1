#include "FixedSizeBox.h"
#include "Components/SizeBox.h"


void UFixedSizeBox::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    createLayout();
    SetWidth(500);
}

void UFixedSizeBox::createLayout(){
    SizeBox = NewObject<USizeBox>(this);
}

void UFixedSizeBox::OverrideChild(UcustomUiComponentBase *item){
    if(!SizeBox){
        createLayout();
    }
    if(item){
        UWidget *widget = item->baseLayoutPointer();
        if(widget){
            child = item;
            SizeBox->AddChild(widget); //Overrides the child inside the sizebox
        }
    }
}

void UFixedSizeBox::SetWidth(int pixels){
    if(SizeBox){
        pixels = validatePixels(pixels);
        SizeBox->SetWidthOverride(pixels);
    }
}

int UFixedSizeBox::validatePixels(int num){
    num = std::max(num, 1);
    return num;
}

bool UFixedSizeBox::dispatchClick(){
    return child != nullptr && child->dispatchClick();
}


void UFixedSizeBox::setVisible(bool visible){
    if(child){
        child->setVisible(visible);
    }
}