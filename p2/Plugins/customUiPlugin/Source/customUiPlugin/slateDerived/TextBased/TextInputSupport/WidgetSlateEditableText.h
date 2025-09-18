#pragma once

#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"


#include "Components/Widget.h"
#include "Components/SizeBox.h"
#include "customUiPlugin/slate/MeshData2D/BaseMeshData/MeshDataWrap/SlateMeshDataPolygon.h"
#include "customUiPlugin/slate/MeshData2D/sharedContainer/SlatePolygonMap.h"
#include "customUiPlugin/baseInterface/WidgetHelper.h"
#include "customUiPlugin/UserInputTracking/UserInput.h"

#include "customUiPlugin/slateDerived/TextBased/Text/WidgetSlateText.h"

#include "WidgetSlateEditableText.generated.h"


/// --- Same as base class but listens for text input ---
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetSlateEditableText : public UWidgetSlateText{
    GENERATED_BODY()

public:
    void SwitchTextToLayer(int layer);

    virtual bool dispatchClick() override;
    virtual void Tick(float deltatime) override;


protected:
    

    // ---- keyboard handling ----
    virtual void dispatchUserInput(UserInput &input) override;

private:
    

    void SetFocusKeyboard(bool bEnabled);
    bool bKeyBoardFocusFlag = false;
};