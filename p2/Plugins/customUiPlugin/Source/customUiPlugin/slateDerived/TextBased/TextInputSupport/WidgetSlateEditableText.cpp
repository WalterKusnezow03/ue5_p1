#include "WidgetSlateEditableText.h"
#include "customUiPlugin/ui/Widgets/buttons/colors/UiColors.h"


void UWidgetSlateEditableText::SwitchTextToLayer(int layer){
    SlateMeshDataPolygon &oldPolygon = PolygonAtTextLayer();
    textLayer = layer;
    SlateMeshDataPolygon &newPolygon = PolygonAtTextLayer();

    newPolygon.CopyTextData(oldPolygon);
    oldPolygon.ClearText();
}



bool UWidgetSlateEditableText::dispatchClick(){
    bool result = Super::dispatchClick();
    SetFocusKeyboard(result);
    return result;
}


void UWidgetSlateEditableText::SetFocusKeyboard(bool bEnabled)
{
    bKeyBoardFocusFlag = bEnabled;
}



// --- Key board handling ---
void UWidgetSlateEditableText::dispatchUserInput(UserInput &input){
    //UiDebugHelper::showScreenMessage("Editable text keyboard dispatch", FColor::Orange);
    if (!bKeyBoardFocusFlag)
    {
        return;
    }

    const TArray<TCHAR> &pressed = input.GetPressedKeys();
    SlateMeshDataPolygon &polygon = PolygonAtTextLayer();
    if (pressed.Num() > 0)
    {
        for(TCHAR c : pressed){
            polygon.AppendChar(c);
        }
    }
    if(input.PressedBackspace()){
        polygon.RemoveChar();
    }
}

void UWidgetSlateEditableText::Tick(float deltatime)
{
    Super::Tick(deltatime);
        
}


