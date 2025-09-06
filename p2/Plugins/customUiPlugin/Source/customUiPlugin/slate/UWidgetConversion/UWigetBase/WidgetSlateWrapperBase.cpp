#include "WidgetSlateWrapperBase.h"
#include "customUiPlugin/slate/base/SlateWidgetBase.h"
#include "customUiPlugin/Private/Debug/UiDebugHelper.h"

void UWidgetSlateWrapperBase::InitSharedPolygonMapPtrIfNeeded(){
    if(!polygonMap.IsValid()){
        polygonMap = MakeShared<SlatePolygonMap>();
    }
}





void UWidgetSlateWrapperBase::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MySlateWidget.Reset(); //sets ptr to nullptr
}

TSharedRef<SWidget> UWidgetSlateWrapperBase::RebuildWidget()
{
    InitSharedPolygonMapPtrIfNeeded();
    /*
    Src Code SizeBox:

    TSharedRef<SWidget> USizeBox::RebuildWidget()
    {
        MySizeBox = SNew(SBox);

        if (GetChildrenCount() > 0)
        {
            Cast<USizeBoxSlot>(GetContentSlot())->BuildSlot(MySizeBox.ToSharedRef());
        }

        return MySizeBox.ToSharedRef();
    }
    */

    //get SBox from Parent: USizeBox
    TSharedRef<SBox> base = StaticCastSharedRef<SBox>(Super::RebuildWidget());

    TSharedRef<SWidget> t = TRebuildWidget<SSlateWidgetBase>(
        MySlateWidget, //created and overriden
        base //Sbox ref, child added
    ); 

    //Construct a single time, mesh data wont be deleted.
    if(!bWasConstructed){
        ConstructWidget(); //do not remove this, can call derived method!
        UpdateSizeBoxBoundsIfMeshDataMarkedDirty(); //very important
        bWasConstructed = true;
    }

    return t;
}



void UWidgetSlateWrapperBase::Tick(float deltatime){
    if(TickAllowed()){
        if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
            ptr->Tick(deltatime); //updates cursor position
        }
        if(polygonMap.IsValid()){
            polygonMap->Tick(deltatime);
        }

        UpdateSizeBoxBoundsIfMeshDataMarkedDirty();
    }
    
}

void UWidgetSlateWrapperBase::UpdateSizeBoxBoundsIfMeshDataMarkedDirty(){
    if(polygonMap.IsValid()){
        if(polygonMap->BoundsUpdated()){
            SetWidthAndHeight(polygonMap->Bounds());
        }
    }
    
}






bool UWidgetSlateWrapperBase::dispatchClick(){
    if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
        return ptr->dispatchClick();
    }
    return false;
}



void UWidgetSlateWrapperBase::ConstructWidget(){
    //to be overriden!
    //DEBUG HERE
    if(bDebugPolygon){
        if(polygonMap.IsValid()){
            polygonMap->DebugCreatePolygons();
        }
    }
}


void UWidgetSlateWrapperBase::SetWidthAndHeight(FVector2D size){
    size.X = std::max(std::abs(size.X), 1.0);
    size.Y = std::max(std::abs(size.Y), 1.0);
    SetWidthOverride(size.X);
    SetHeightOverride(size.Y);
    SynchronizeProperties();

    FString msg = FString::Printf(
        TEXT("UWidgetSlateWrapperBase scale override (%.2f %.2f)"),
        size.X, size.Y
    );
    UiDebugHelper::logMessage(msg);
}

///Temporary reference! - use one at a time!
SlateMeshDataPolygon &UWidgetSlateWrapperBase::FindFromMap(int layer){
    InitSharedPolygonMapPtrIfNeeded();
    return polygonMap->FindPolygonByLayerInternal(layer);
}