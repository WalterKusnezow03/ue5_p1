#pragma once

#include "CoreMinimal.h"
#include "AutoBoxBase.h"
#include "Components/GridPanel.h"


#include "GridBox.generated.h"

class UPlayerUiBase;

/// @brief is a grid, can add childs to the next free position, or at a wanted position
/// can remove columns or rows, can increase its size, but new items exceeding the size will be added
/// as a new row
UCLASS()
class CUSTOMUIPLUGIN_API UGridBox : public UAutoBoxBase {
    GENERATED_BODY()

public:
    using UAutoBoxBase::init;
    virtual void init() override;
    virtual void init(int i, int j);

public:
    virtual void AddChild(UcustomUiComponentBase *item) override;
    virtual void AddChild(UWidget *any) override;

    virtual void AddChild(UcustomUiComponentBase *item, int i, int j);
    virtual void AddChild(UWidget *widget, int i, int j);

    void AddRow(TArray<UcustomUiComponentBase *> &items);

    virtual UWidget *baseLayoutPointer() override{
        return gridBoxLayout;
    }

    /// @brief removes a row by an item searched
    /// @param item 
    void RemoveRow(UcustomUiComponentBase *item);

    TArray<UcustomUiComponentBase *> RemovedItemsFromRemoveRow(UcustomUiComponentBase *item);

    /// @brief removes a row by index
    /// @param numRow 
    void RemoveRow(int numRow);

    ///@brief shows or hides a row if an item is found, hides or shows row the item belongs to
    void SetRowVisible(UcustomUiComponentBase *item, bool show);

    ///@brief hides / collapses or shows a row by index if in bound
    void SetRowVisible(int i, bool show);

    ///@brief hides / collapses or shows all rows
    void SetAllRowsVisible(bool show);


    ///@brief removes a child from click listening
    virtual void RemoveChild(UcustomUiComponentBase *item) override;
    virtual void RemoveChild(UWidget *item) override;

protected:
    virtual void UpdatePadding(UWidget *item) override;

private:
    
    bool IsFull();
    /// @brief returns if is full and writes free index if is not full
    bool IsFull(int &iOut, int &jOut);

    UPROPERTY()
    UGridPanel *gridBoxLayout = nullptr;

    //UPROPERTY()
    //Row<Column<widget>>
    TArray<TArray<UcustomUiComponentBase*>> trackedGrid;

    void SetupGrid();

    void FillArrayIncluding(int i, int j);
    int NumRows();
    int NumColumns();

    bool IsFree(int i, int j);
    bool IndexIsValid(int i, int j);
    bool RowIndexValid(int i);
    bool RowAtIndexEmpty(int i);

    bool Find(UcustomUiComponentBase *item, int &iOut, int &jOut);
};