#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/baseParent/UiComponentParent.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"
#include "customUiPlugin/slateDerived/Shapes/ArrowWidget/WidgetArrowBase.h"


#include "VerticalDropDownBase.generated.h"

/// @brief removing childs wont be supported for now.
/// Selectable items in dropdown list
UCLASS()
class CUSTOMUIPLUGIN_API UVerticalDropDownBase : public UVbox{
GENERATED_BODY()

public:
    virtual void init() override;

    //debug
    virtual void Tick(float deltatime) override {
        Super::Tick(deltatime);


        ////////// -------- ISSUE HERE ! -------- //////////

        //fixes the issue of selected item! Is not ticked!!!
        //if(selectedCopy){
        //    selectedCopy->Tick(deltatime);
        //}
    }

    ///dispatches a click to all children
    virtual bool dispatchClick() override;

    ///@brief may be -1 if none hit in latest clikc dispatch - is resettet each click.
    int ListIndexHitFromClickDispatch();

    ///@brief returns interface at a certain index from pickable list or nullptr if index oob / list
    ///not setup.
    IBaseUiInterface *BaseInterfaceFromListAtIndex(int index);

    /// @brief adds the child to the selectable list.
    /// adds child tracked to Click, Tick and Visibilty Dispatch
    /// CAUTION: UcustomUiComponentBase *DuplicateWidgetInitialized(UObject*); MUST BE IMPLEMENTED!
    virtual void AddChild(IBaseUiInterface *item) override;

    void SelectIndex(int index);

    IBaseUiInterface *SelectedItemInterface(){
        return selectedItem;
    }

    void Debug();

    ///access pickable items, pointers read only.
    const TArray<IBaseUiInterface *> &AccessListInternalItemsTmp() const;

    void HidePickerMenu();

protected:
    int indexHitFromClickDispatch = -1;

    UcustomUiComponentBase *CreateDuplicate(IBaseUiInterface *interface);

    void createLayout();
    void CreateTopSelectionBar();


    void SwitchMenuVisibilty();

    /// @brief actual item from list
    IBaseUiInterface *selectedItem = nullptr;

    /// @brief copy of item
    IBaseUiInterface *selectedCopy = nullptr;

    UPROPERTY() 
    UHbox *topSelection = nullptr;

    UPROPERTY() 
    UVbox *selectableList = nullptr;




    UPROPERTY()
    UTextButton *topTextButton = nullptr;

    UPROPERTY()
    UWidgetArrowBase *topArrow = nullptr;

private:
    TArray<IBaseUiInterface *> fallbackArray;




    int topSelectedResX = 100;
    int topResY = 40;
    
    void RescaleAsTopItem(IBaseUiInterface *item);
    void RescaleAsListItem(IBaseUiInterface *item);
    void RescaleIfPossible(IBaseUiInterface *item, FVector2D scale);
    void RescaleXIfPossible(IBaseUiInterface *item, int scaleX);
};