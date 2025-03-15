#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/CanvasPanel.h"
#include "Components/BackgroundBlur.h"
#include "Components/VerticalBox.h"

class UPlayerUi;

class P2_API CanvasScreen : public customUiComponentBase {

public:
    CanvasScreen(){
        baseCanvas = nullptr;
    }
    ~CanvasScreen(){
        baseCanvas = nullptr;
    }

    CanvasScreen(UPlayerUi &uiIn);

    


    virtual void setVisible(bool visible) override;
    virtual UWidget *baseLayoutPointer() override;

protected:
    UCanvasPanel *baseCanvas = nullptr;

    void createBaseCanvas();
    void updateCanvasScale(UWorld *world);



    UBackgroundBlur *backgroundBlur = nullptr;
    void createBackgroundBlur();

};