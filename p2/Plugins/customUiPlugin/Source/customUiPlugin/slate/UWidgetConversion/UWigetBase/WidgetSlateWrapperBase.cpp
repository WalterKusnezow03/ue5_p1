#include "WidgetSlateWrapperBase.h"
#include "customUiPlugin/slate/base/SlateWidgetBase.h"
#include "customUiPlugin/Private/Debug/UiDebugHelper.h"


void UWidgetSlateWrapperBase::InitSharedPolygonMapPtrIfNeeded(){
    if(!polygonMap.IsValid()){
        polygonMap = MakeShared<SlatePolygonMap>();
    }
}

void UWidgetSlateWrapperBase::PostInitProperties(){
    /*
    Debug log for better understanding:

    LogTemp: UWidgetSlateWrapperBase debug lifecycle - PostInitProperties id(0)
    LogTemp: UWidgetSlateWrapperBase debug lifecycle - PostInitProperties id(1)
    LogTemp: UWidgetSlateWrapperBase::SetResolution, debug lifecycle (1) polygonmap valid and constructed X=50.000 Y=50.000
    LogTemp: UWidgetSlateWrapperBase::SetResolution, debug lifecycle (1) polygonmap valid and constructed X=8.294 Y=40.000
    LogTemp: UWidgetSlateWrapperBase debug lifecycle - RebuildWidget id(0)
    LogTemp: UWidgetSlateWrapperBase debug lifecycle - RebuildWidget id(1)
    
    //--> first: PostInitProperties: Create Widget here.
    //--> second: Set Resolution now is valid because mesh data already will be created
    //--> third: RebuildWidget -> Rebuild widget can be done now without any issues, mesh data created
    //--> if first and second would switch: Creating the Widget as NewObject<T> would not allow
    //resize right after that.

    //resize / set resolution still managed in tick 
    //apparently it still is weird.
    */



    //happens before rebuild widget.
    InitSharedPolygonMapPtrIfNeeded();
    /*UiDebugHelper::logMessage(
        FString::Printf(TEXT("UWidgetSlateWrapperBase debug lifecycle - PostInitProperties id(%d)"), id)
    );*/

    //---- Must be called here: earliest entry point before building widget ----
    //since post init properties is 
    //callid before rebuild widget:
    //Construct a single time, mesh data wont be deleted.
    if(!bWasConstructed){
        ConstructWidget(); //do not remove this, can call derived method!
        //UpdateSizeBoxBoundsIfMeshDataMarkedDirty(); //very important
        bWasConstructed = true;
    }
    UpdateResolutionFromProperty();

    Super::PostInitProperties();
}

void UWidgetSlateWrapperBase::BeginDestroy(){
    Super::BeginDestroy();
}

void UWidgetSlateWrapperBase::ReleaseSlateResources(bool bReleaseChildren)
{
    MySlateWidget.Reset(); //sets ptr to nullptr
    Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> UWidgetSlateWrapperBase::RebuildWidget()
{
    InitSharedPolygonMapPtrIfNeeded();
    
    //get SBox from Parent: USizeBox
    TSharedRef<SBox> base = StaticCastSharedRef<SBox>(Super::RebuildWidget());

    TSharedRef<SWidget> t = TRebuildWidget<SSlateWidgetBase>(
        MySlateWidget, //created and overriden
        base //Sbox ref, child added
    ); 

    return t;
}


void UWidgetSlateWrapperBase::Tick(float deltatime){
    if(TickAllowed()){ //if was constructed.
        if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
            ptr->Tick(deltatime); //updates cursor position
        }
        if(polygonMap.IsValid()){
            polygonMap->Tick(deltatime);
        }

        //ProcessScalingTasks();
        //UpdateSizeBoxBoundsIfMeshDataMarkedDirty();

        IsHoveredTick();
    }
    if(bDebugTickLog){
        UiDebugHelper::showScreenMessage("UWidgetSlateWrapper base tick log", FColor::Green);
    }
}

void UWidgetSlateWrapperBase::UpdateResolutionFromProperty(){
    FVector2D copy = desiredResolution;
    copy.X = std::max(100.0,copy.X);
    copy.Y = std::max(100.0,copy.Y);
    SetWidthOverride(copy.X);
    SetHeightOverride(copy.Y);
    
    if(polygonMap.IsValid()){
        polygonMap->ScaleToResolutionImmidiate(copy);
    }
    SynchronizeProperties();
}





void UWidgetSlateWrapperBase::IsHoveredTick(){
    if(Super::IsHovered()){
        OnHover();
        return;
    }

    if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
        if(ptr->IsHovered()){
            OnHover();
            return;
        }
    }
}

void UWidgetSlateWrapperBase::OnHover(){
    //empty here.
}


void UWidgetSlateWrapperBase::SetCursorColorEnabled(bool flag){
    if(polygonMap.IsValid()){
        polygonMap->SetCursorColorEnabled(flag);
        //UiDebugHelper::logMessage("SetCursorColor enaled UWidgetSlateWrapperBase");
    }
}





bool UWidgetSlateWrapperBase::dispatchClick(const FVector2D &pos){
    if(!markedVisible()){
        return false;
    }
    if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
        return ptr->dispatchClick(pos);
    }
    return false;
}



void UWidgetSlateWrapperBase::ConstructWidget(){
    //to be overriden!
    //DEBUG HERE
    if(bDebugPolygon && false){
        if(polygonMap.IsValid()){
            polygonMap->DebugCreatePolygons();
        }
    }
    
}




///Temporary reference! - use one at a time!
SlateMeshDataPolygon &UWidgetSlateWrapperBase::FindFromMap(int layer){
    InitSharedPolygonMapPtrIfNeeded();
    return polygonMap->FindPolygonByLayerInternal(layer);
}