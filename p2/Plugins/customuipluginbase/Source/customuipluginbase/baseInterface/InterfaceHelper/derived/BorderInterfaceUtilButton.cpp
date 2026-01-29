#include "BorderInterfaceUtilButton.h"


BorderInterfaceUtilButton::BorderInterfaceUtilButton(){

}

BorderInterfaceUtilButton::~BorderInterfaceUtilButton(){

}

void BorderInterfaceUtilButton::Tick(float deltatime){
    if(IsChecked()){
        if(timeLeft > 0.0f){
            timeLeft -= deltatime;
        }else{
            ResetCheckedState();
        }
    }
}


bool BorderInterfaceUtilButton::dispatchClick(const FVector2D &position){
    if(BorderInterfaceUtil::dispatchClick(position)){
        timeLeft = resetTime;
        return true;
    }
    return false;
}