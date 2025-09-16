#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/baseParent/UiComponentParent.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"

#include "Components/ComboBox.h"

#include "VerticalDropDownBase.generated.h"

/// @brief removing childs wont be supported for now.
/// Selectable items in dropdown list
UCLASS()
class CUSTOMUIPLUGIN_API UVerticalDropDownBase : public UVbox{
GENERATED_BODY()

public:
    virtual void init() override;


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

protected:
    int indexHitFromClickDispatch = -1;

    UcustomUiComponentBase *CreateDuplicate(IBaseUiInterface *interface);

    void createLayout();
    void SwitchMenuVisibilty();

    IBaseUiInterface *selectedItem = nullptr;

    UPROPERTY() 
    UHbox *topSelection = nullptr;

    UPROPERTY() 
    UVbox *selectableList = nullptr;

    UPROPERTY()
    UTextButton *topTextButton = nullptr;

private:
    TArray<IBaseUiInterface *> fallbackArray;
};