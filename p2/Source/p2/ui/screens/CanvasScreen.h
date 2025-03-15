#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "Components/CanvasPanel.h"
#include "Components/BackgroundBlur.h"
#include "Components/VerticalBox.h"

#include "CanvasScreen.generated.h"

class UPlayerUi;

UCLASS()

class P2_API UCanvasScreen : public UcustomUiComponentBase {
    GENERATED_BODY()
public:

    virtual void init(UPlayerUi &refin);

    virtual void setVisible(bool visible) override;
    virtual UWidget *baseLayoutPointer() override;

protected:
    UCanvasPanel *baseCanvas = nullptr;

    void createBaseCanvas();
    void updateCanvasScale(UWorld *world);



    UBackgroundBlur *backgroundBlur = nullptr;
    void createBackgroundBlur();

};