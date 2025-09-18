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
    if(TickAllowed()){ //if was constructed.
        if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
            ptr->Tick(deltatime); //updates cursor position
        }
        if(polygonMap.IsValid()){
            polygonMap->Tick(deltatime);
        }

        if(task.MarkedDirty() && polygonMap.IsValid()){
            polygonMap->ScaleToResolutionImmidiate(task.scaleToSet);
            UiDebugHelper::logMessage(
                FString::Printf(TEXT("UWidgetSlateWrapperBase process Scale set task %s"), 
                *task.scaleToSet.ToString())
            );
        }
        if(taskRawXY.MarkedDirty()){
            SetResolution(taskRawXY.scaleToSet);
        }
        if(taskRawX.MarkedDirty()){
            SetResolutionXUniform(taskRawX.scaleToSet.X);
        }

        UpdateSizeBoxBoundsIfMeshDataMarkedDirty();
    }
    if(bDebugTickLog){
        UiDebugHelper::showScreenMessage("UWidgetSlateWrapper base tick log", FColor::Green);
    }
}

void UWidgetSlateWrapperBase::UpdateSizeBoxBoundsIfMeshDataMarkedDirty(){
    if(polygonMap.IsValid()){

        //testing needed here!
        if (polygonMap->BoundsUpdated())
        {
            UiDebugHelper::logMessage("UWidgetSlateWrapperBase bounds update 2");
            SetWidthAndHeightSizeBox(polygonMap->Bounds());
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
    if(bDebugPolygon && false){
        if(polygonMap.IsValid()){
            polygonMap->DebugCreatePolygons();
        }
    }
    
}




FVector2D UWidgetSlateWrapperBase::GetResolution(){
    if(MySlateWidget.IsValid()){
        FGeometry Geometry = MySlateWidget->GetCachedGeometry();
        FVector2D LocalSize = Geometry.GetLocalSize();
        return LocalSize;
    }
    return FVector2D(-1, -1);
}

void UWidgetSlateWrapperBase::SetResolution(FVector2D scale){
    
    
    if(bWasConstructed && polygonMap.IsValid()){
        task.Update(scale);
    }else{
        taskRawXY.Update(scale);
    }
    
    
}

void UWidgetSlateWrapperBase::SetResolutionXUniform(int scale){
    if(polygonMap.IsValid()){
        if(bWasConstructed){
            FVector2D bounds = polygonMap->Bounds();
            if(bounds.X <= 0.0f){
                return;
            }

            //a * xCurrent = target
            //a = target / xCurrent
            float a = scale / bounds.X;

            bounds *= a;
            SetResolution(bounds);
            return;
        }
    }
    taskRawX.Update(FVector2D(scale,0));
}



// internal. Dont touch. Fixes bounds for layout inside hbox etc.
void UWidgetSlateWrapperBase::SetWidthAndHeightToUniformBoundsSizeBox(){
    if(polygonMap.IsValid()){
        FVector2D bounds = polygonMap->Bounds();

        float max = std::max(bounds.X, bounds.Y);
        SetWidthAndHeightSizeBox(FVector2D(max, max));
    }
}   




void UWidgetSlateWrapperBase::SetWidthAndHeightSizeBox(FVector2D size){
    size.X = std::max(std::abs(size.X), 1.0);
    size.Y = std::max(std::abs(size.Y), 1.0);
    SetWidthOverride(size.X);
    SetHeightOverride(size.Y);
    SynchronizeProperties();

    FString msg = FString::Printf(
        TEXT("UWidgetSlateWrapperBase scale override (%.2f %.2f)"),
        size.X, size.Y
    );
    if(true)
        UiDebugHelper::logMessage(msg);
}

///Temporary reference! - use one at a time!
SlateMeshDataPolygon &UWidgetSlateWrapperBase::FindFromMap(int layer){
    InitSharedPolygonMapPtrIfNeeded();
    return polygonMap->FindPolygonByLayerInternal(layer);
}