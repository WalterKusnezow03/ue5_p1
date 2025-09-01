#include "GridBox.h"
#include "Components/GridSlot.h"


void UGridBox::init(){
    if(WAS_INIT_FLAG){
        return;
    }
    init(2, 5); //some number as example
}

void UGridBox::init(int i, int j){
    if(WAS_INIT_FLAG){
        return;
    }
    Super::init();
    SetupGrid();
    FillArrayIncluding(i - 1, j - 1); //one less because otherwise constructor is unexpected
}


void UGridBox::SetupGrid(){
    gridBoxLayout = NewObject<UGridPanel>(this);
}


void UGridBox::FillArrayIncluding(int iFill, int jFill){
    int iTargetNum = std::abs(iFill) + 1; //target size
    int jTargetNum = std::abs(jFill) + 1;

    //already filled cases
    if(iTargetNum < NumRows() && jTargetNum < NumColumns()){
        return;
    }

    //fill columns
    for (int i = 0; i < trackedGrid.Num(); i++){
        TArray<UcustomUiComponentBase *> &ref = trackedGrid[i];
        if(ref.Num() < jTargetNum){
            int diff = jTargetNum - ref.Num();
            for (int j = 0; j < diff; j++){
                ref.Add(nullptr); //initied with nullptr!
            }
        }
    }

    // fill completly missing rows, inited with nullptr
    for (int i = 0; i < (iTargetNum - trackedGrid.Num()); i++)
    {
        TArray<UcustomUiComponentBase *> addArray;
        for (int j = 0; j < jTargetNum; j++){
            addArray.Add(nullptr);
        }

        trackedGrid.Add(addArray);
    }
}




int UGridBox::NumRows(){
    return trackedGrid.Num();
}

int UGridBox::NumColumns(){
    return trackedGrid.Num() > 0 ? trackedGrid[0].Num() : 0;
}


void UGridBox::AddChild(UcustomUiComponentBase *item){
    if(item){
        int freeI = 0;
        int freeJ = 0;
        if(IsFull(freeI, freeJ)){
            //always adds a new row.
            FillArrayIncluding(NumRows(), NumColumns() - 1); //push row down!
            freeI = NumRows() - 1;
            freeJ = 0;
        }
        AddChild(item, freeI, freeJ);
    }
}

void UGridBox::AddChild(UWidget *any){
    int freeI = 0;
    int freeJ = 0;
    if(IsFull(freeI, freeJ)){
        //always adds a new row.
        FillArrayIncluding(NumRows(), NumColumns() - 1); //push row down!
        freeI = NumRows() - 1;
        freeJ = 0;
    }
    AddChild(any, freeI, freeJ);
}




void UGridBox::AddChild(UcustomUiComponentBase *item, int i, int j){
    if(item && IsFree(i,j)){
        Super::AddChild(item); //add to click dispatcher!!

        //make grid valid, add slot, then to logic grid
        FillArrayIncluding(i, j);
        AddChild(item->baseLayoutPointer(), i, j);
        trackedGrid[i][j] = item;
    }
}

void UGridBox::AddChild(UWidget *item, int i, int j){
    if(item && gridBoxLayout){
        

        if(IsFree(i,j)){
            // Add to grid
            if (UGridSlot* Slot = Cast<UGridSlot>(gridBoxLayout->AddChild(item)))
            {
                Slot->SetRow(i);
                Slot->SetColumn(j);
            }
            UpdatePadding(item);
        }
    }
}

void UGridBox::AddRow(TArray<UcustomUiComponentBase *> &items){
    if(items.Num() > 0){
        int lowestRow = NumRows(); //index will be valid.
        FillArrayIncluding(lowestRow, items.Num() - 1);
        for (int i = 0; i < items.Num(); i++){
            UcustomUiComponentBase *item = items[i];
            if(item){
                AddChild(item, lowestRow, i);
            }
        }
    }
}





bool UGridBox::IsFull(){
    int i = 0;
    int j = 0;
    return IsFull(i, j); //ignored out
}

bool UGridBox::IsFull(int &iOut, int &jOut){
    for (int i = 0; i < trackedGrid.Num(); i++){
        TArray<UcustomUiComponentBase *> &current = trackedGrid[i];
        for (int j = 0; j < current.Num(); j++){
            if(current[j] == nullptr){
                iOut = i;
                jOut = j;
                return false;
            }
        }
    }
    return true;
}

bool UGridBox::IsFree(int i, int j){
    if(IndexIsValid(i,j)){
        return trackedGrid[i][j] == nullptr;
    }
    return false;
}

bool UGridBox::IndexIsValid(int i, int j){
    return i >= 0 && i < NumRows() && j >= 0 && j < NumColumns();
}

bool UGridBox::RowIndexValid(int i){
    return i >= 0 && i < NumRows();
}

bool UGridBox::RowAtIndexEmpty(int i){
    if(RowIndexValid(i)){
        //check all "columns" in the row, for being clear
        for (int j = 0; j < trackedGrid[i].Num(); j++)
        {
            if(!IsFree(i,j)){
                return false;
            }
        }
        return true;
    }
    return false;
}

bool UGridBox::Find(UcustomUiComponentBase *item, int &iOut, int &jOut){
    if(item){
        for (int i = 0; i < trackedGrid.Num(); i++){
            TArray<UcustomUiComponentBase *> &row = trackedGrid[i];
            for (int j = 0; j < row.Num(); j++){
                if(row[j] == item){
                    iOut = i;
                    jOut = j;
                    return true;
                }
            }
        }
    }
    return false;
}

///@brief removes a child from click listening and from layout
void UGridBox::RemoveChild(UcustomUiComponentBase *item){
    Super::RemoveChild(item); //remove from dispatcher
    if(UWidget *layoutPointer = item->baseLayoutPointer()){
        RemoveChild(layoutPointer);
    }
}

void UGridBox::RemoveChild(UWidget *item){

    //remove from UMG Layout
    if(gridBoxLayout){
        if(gridBoxLayout->RemoveChild(item)){
            //sucessfully remove child
        }
    }

    //find in tracked grid and remove, will remove the whole row if empty
    for (int i = 0; i < trackedGrid.Num(); i++){
        TArray<UcustomUiComponentBase *> &ref = trackedGrid[i];
        for (int j = 0; j < ref.Num(); j++){
            UcustomUiComponentBase *current = ref[j];
            if(current && current->baseLayoutPointer() == item){
                ref[j] = nullptr;
            
                if(RowAtIndexEmpty(j)){
                    RemoveRow(j);
                    return; //break process
                }
            }
        }
    }
}






void UGridBox::RemoveRow(UcustomUiComponentBase *item){
    int i = 0;
    int j = 0;
    if(Find(item, i, j)){
        RemoveRow(i);
    }
}

TArray<UcustomUiComponentBase *> UGridBox::RemovedItemsFromRemoveRow(
    UcustomUiComponentBase *item
){
    TArray<UcustomUiComponentBase *> copyRow; //removed items
    int i = 0;
    int j = 0;
    if(Find(item, i, j)){
        copyRow = trackedGrid[i]; ///copy data before removal!
        RemoveRow(i);
    }

    //clean from nullptr
    TArray<UcustomUiComponentBase *> cleaned;
    for (int index = 0; index < copyRow.Num(); index++){
        UcustomUiComponentBase *current = copyRow[index];
        if(current != nullptr){
            cleaned.Add(current);
        }
    }
    return cleaned;
}

//method will remove from tracked map
//and shift children
void UGridBox::RemoveRow(int indexRow){
    if(RowIndexValid(indexRow)){
        //Remove array from grid
        TArray<UcustomUiComponentBase *> toRemoveCopy = trackedGrid[indexRow];
        trackedGrid.RemoveAt(indexRow); //everything below shifts one up. 
        //IndexRow is now valid index to acess below items


        //remove pointers from u grid panel
        for (int i = 0; i < toRemoveCopy.Num(); i++){
            if(gridBoxLayout){
                if(UcustomUiComponentBase *wrapper = toRemoveCopy[i]){
                    if(UWidget *ptr = wrapper->baseLayoutPointer()){
                        gridBoxLayout->RemoveChild(ptr);
                    }
                }
            }
        }


        //shift lower elements up, starting at removed row, lower shifted up.
        for (int i = indexRow; i < trackedGrid.Num(); i++){
            TArray<UcustomUiComponentBase *> &toShiftRow = trackedGrid[i];

            for (int j = 0; j < toShiftRow.Num(); j++){
                UcustomUiComponentBase *child = toShiftRow[j];
                if(child){
                    UWidget *uChild = child->baseLayoutPointer();
                    if(uChild){
                        if (UGridSlot *Slot = Cast<UGridSlot>(uChild->Slot))
                        {
                            // Just sequentially assign rows, keep same column
                            Slot->SetRow(i); //i is at modified row.
                        }
                    }
                }


                
            }
        }
    }
}



// Show hide
void UGridBox::SetRowVisible(UcustomUiComponentBase *item, bool show){
    int i = 0;
    int j = 0;
    if(Find(item, i, j)){
        SetRowVisible(i, show);
    }
}

void UGridBox::SetRowVisible(int i, bool show){
    if(i >= 0 && i < NumRows()){
        TArray<UcustomUiComponentBase*> &row = trackedGrid[i]; ///copy data before removal!
        for (int index = 0; index < row.Num(); index++){
            UcustomUiComponentBase *itemCurrent = row[index];
            if(itemCurrent){
                itemCurrent->setVisible(show);
            }
        }
    }
}

void UGridBox::SetAllRowsVisible(bool show){
    for (int i = 0; i < NumRows(); i++){
        SetRowVisible(i, show);
    }
}


// -- padding updates --
void UGridBox::UpdatePadding(UWidget *widget){
    if(widget){
        if(UGridSlot* Slot = Cast<UGridSlot>(widget->Slot)){
            Slot->SetPadding(makePadding());
        }
    }
}
