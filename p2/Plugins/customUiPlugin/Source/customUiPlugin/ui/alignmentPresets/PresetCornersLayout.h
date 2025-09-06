#pragma once

#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
#include "PresetCornersLayout.generated.h"

class UCanvasScreen;

/**
 * preset corners layout with vertical boxes
 */

UCLASS()
class CUSTOMUIPLUGIN_API UPresetCornersLayout : public UcustomUiComponentBase{

    GENERATED_BODY()

public:
    using UcustomUiComponentBase::init;
    void init(UCanvasScreen *parentPanelIn);

    void addChildToTopLeft(UWidget *any);
    void addChildToTopRight(UWidget *any);
    void addChildToBottomLeft(UWidget *any);
    void addChildToBottomRight(UWidget *any);
    void addChildToTopCenter(UWidget *any);

    void addChildToTopLeft(UcustomUiComponentBase *any);
    void addChildToTopRight(UcustomUiComponentBase *any);
    void addChildToBottomLeft(UcustomUiComponentBase *any);
    void addChildToBottomRight(UcustomUiComponentBase *any);
    void addChildToTopCenter(UcustomUiComponentBase *any);

    //custom click dispatch and visibility
    virtual bool dispatchClick() override;
    virtual void SetVisible(bool visible) override;
    virtual void Tick(float deltatime) override;

private:
    void createSubLayouts();

    void addChildTo(UVbox *box, UWidget *any);
    void addChildTo(UVbox *box, UcustomUiComponentBase *any);

    UPROPERTY()
    UVbox *topLeft = nullptr;

    UPROPERTY()
    UVbox *topRight = nullptr;

    UPROPERTY()
    UVbox *bottomLeft = nullptr;

    UPROPERTY()
    UVbox *bottomRight = nullptr;
    
    UPROPERTY()
    UVbox *topCenter = nullptr;

    UPROPERTY()
    UCanvasPanel *parentPanel = nullptr;

    void addToParentPanelAndInit(UcustomUiComponentBase *item, FVector2D anchor, FVector2D alignment);

    UPROPERTY()
    TArray<UcustomUiComponentBase *> elements;
};