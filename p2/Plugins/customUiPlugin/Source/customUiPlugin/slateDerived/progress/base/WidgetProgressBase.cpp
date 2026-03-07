#include "WidgetProgressBase.h"





void UWidgetProgressBase::SetProgress(float num){
    progressScalar = ClampProgress(num);
}



void UWidgetProgressBase::ResetProgress(){
    SetProgress(1.0f);
}


float UWidgetProgressBase::ClampProgress(float num){
    num = std::max(num, 0.0f);
    num = std::min(num, 1.0f);
    return num;
}


