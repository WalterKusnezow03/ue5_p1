#include "WidgetSlateEditableText.h"
#include "customUiPlugin/ui/Widgets/buttons/colors/UiColors.h"

void UWidgetSlateEditableText::ConstructWidget(){
    Super::ConstructWidget();

    SlateMeshDataPolygon &cursorPolygon = PolygonAtTextCursorLayer();
    SlateMeshData &mesh = cursorPolygon.MeshDataRef();

    FVector2D tl(0, 0);
    FVector2D br(1, 1);
    mesh.AppendQuad(tl, br);
    cursorPolygon.SetFullColor(UiColors::buttonWhite);

    SlateMeshDataPolygon &hint = PolygonAtHintTextLayer();
    hint.SetText("Type...");

    SlateTextBase &textInternal = hint.GetSlateText();
    FVector2D maxBoundText = MaxTextSizeDefault();
    textInternal.SetFitMaxSize(maxBoundText);
    
    
    
    FLinearColor linColorGrey = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
    FColor color = linColorGrey.ToFColor(true);
    textInternal.SetColor(color);
    UpdateTextCursorPosition();
    SetCursorVisible(false);
}

SlateMeshDataPolygon &UWidgetSlateEditableText::PolygonAtTextCursorLayer(){
    return FindFromMap(textLayer + 1); //intop of text
}

SlateMeshDataPolygon &UWidgetSlateEditableText::PolygonAtHintTextLayer(){
    return FindFromMap(textLayer + 2); //intop of text
}

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
    SetCursorVisible(bKeyBoardFocusFlag);
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
    UpdateHintTextVisibilty();
    UpdateTextCursorPosition();
}

void UWidgetSlateEditableText::Tick(float deltatime)
{
    Super::Tick(deltatime);

    if(bKeyBoardFocusFlag){
        deltaTimeIntegrated += deltatime;
        if(deltaTimeIntegrated >= cursorIntervall){
            deltaTimeIntegrated = 0.0f;
            SwitchCursorVisibility();
        }
    }
}

void UWidgetSlateEditableText::SwitchCursorVisibility(){
    SetCursorVisible(!bCursorVisible);
}

void UWidgetSlateEditableText::SetCursorVisible(bool visible){
    SlateMeshDataPolygon &cursorPolygon = PolygonAtTextCursorLayer();
    cursorPolygon.EnableRender(visible);
    bCursorVisible = visible;
}

void UWidgetSlateEditableText::UpdateHintTextVisibilty(){
    bool showHint = TextIsEmpty();
    SlateMeshDataPolygon &hint = PolygonAtHintTextLayer();
    hint.EnableRender(showHint);
    SetCursorVisible(showHint);
}

#include "CoreMath/Matrix/2D/MMatrix2D.h"
void UWidgetSlateEditableText::UpdateTextCursorPosition(){
    if(!bCursorVisible){
        //bugs when returning here.
        //return;
    }

    FVector2D cursorLocation;
    float cursorHeight = 1.0f;
    CursorLocationAndHeigth(cursorLocation, cursorHeight);

    // M = T * S <--lese richtung --
    MMatrix2D T(cursorLocation);
    MMatrix2D S;
    S.Scale(cursorWidth, cursorHeight);

    MMatrix2D M = T * S;

    SlateMeshDataPolygon &cursor = PolygonAtTextCursorLayer();
    cursor.SetRuntimeTransformation(M);
}

void UWidgetSlateEditableText::CursorLocationAndHeigth(
    FVector2D &cursorLocation,
    float &cursorHeight
){
    if(TextIsEmpty()){
        SlateMeshDataPolygon &hintLayer = PolygonAtHintTextLayer();
        FVector2D pivot = hintLayer.SlateTextPivot();
        cursorLocation = pivot; //at front of hint text

        const SlateTextBase &textInternal = hintLayer.GetSlateTextConst();
        FVector2D textBounds = textInternal.Bounds(); //height

        cursorHeight = textBounds.Y;
    }
    else
    {
        SlateMeshDataPolygon &textLayer = PolygonAtTextLayer();
        FVector2D pivot = textLayer.SlateTextPivot();

        const SlateTextBase &textInternal = textLayer.GetSlateTextConst();
        FVector2D textBounds = textInternal.Bounds();

        cursorHeight = textBounds.Y;
        cursorLocation = pivot + FVector2D(textBounds.X, 0.0f); //at end of text
    }
}




bool UWidgetSlateEditableText::TextIsEmpty(){
    SlateMeshDataPolygon &text = PolygonAtTextLayer();
    return text.GetSlateText().Lenght() <= 0;
}


//wenn der text leer ist, muss der cursor an den start von
//dem hint text kommen und dort seine höhe annehmen



