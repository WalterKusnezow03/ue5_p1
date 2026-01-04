#include "WidgetSlateWrapperBase.h"
#include "customUiPlugin/slate/base/SlateWidgetBase.h"
#include "customUiPlugin/Private/Debug/UiDebugHelper.h"

int UWidgetSlateWrapperBase::idGlobal = 0;



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
    id = idGlobal++;
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
        UpdateSizeBoxBoundsIfMeshDataMarkedDirty(); //very important
        bWasConstructed = true;
    }

    Super::PostInitProperties();
}

void UWidgetSlateWrapperBase::BeginDestroy(){
    idGlobal = 0;
    Super::BeginDestroy();
}

void UWidgetSlateWrapperBase::ReleaseSlateResources(bool bReleaseChildren)
{
    MySlateWidget.Reset(); //sets ptr to nullptr
    Super::ReleaseSlateResources(bReleaseChildren);
}

TSharedRef<SWidget> UWidgetSlateWrapperBase::RebuildWidget()
{
    /*UiDebugHelper::logMessage(
        FString::Printf(TEXT("UWidgetSlateWrapperBase debug lifecycle - RebuildWidget id(%d)"), id)
    );*/
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

        ProcessScalingTasks();

        UpdateSizeBoxBoundsIfMeshDataMarkedDirty();

        IsHoveredTick();
    }
    if(bDebugTickLog){
        UiDebugHelper::showScreenMessage("UWidgetSlateWrapper base tick log", FColor::Green);
    }
}

void UWidgetSlateWrapperBase::ProcessScalingTasks(){
    //---- resolution update needed in tick because of racing condition,
    // sometimes the widget doesnt react to the resize, because the polygon map is not
    // created yet. ----

    //the widget will not update if not made one frame later.
    if(polygonMap.IsValid() && task.MarkedDirty()){
        polygonMap->ScaleToResolutionImmidiate(task.scaleToSet);

        if(ResolutionReached(task.scaleToSet)){
            task.MarkExecuted();
            /*UiDebugHelper::logMessage(
                FString::Printf(
                    TEXT("UWidgetSlateWrapperBase process Scale set task %s"), 
                    *task.scaleToSet.ToString()
                )
            );*/
        }
    }
    if(taskRawXY.MarkedDirty()){
        /*UiDebugHelper::logMessage(
            FString::Printf(TEXT("UWidgetSlateWrapperBase raw scale process %s"), 
            *taskRawXY.scaleToSet.ToString())
        );*/
        SetResolution(taskRawXY.scaleToSet);
        taskRawXY.MarkExecuted();
    }
    if(taskRawX.MarkedDirty()){
        SetResolutionXUniform(taskRawX.scaleToSet.X);
        taskRawX.MarkExecuted();
    }
    if(taskRawY.MarkedDirty()){
        SetResolutionYUniform(taskRawY.scaleToSet.Y);
        taskRawY.MarkExecuted();
    }
}

bool UWidgetSlateWrapperBase::ResolutionReached(const FVector2D &target){
    FVector2D current = GetResolution();
    float epsilon = 10;
    current.X -= epsilon;
    current.Y -= epsilon;
    return current.X >= target.X && current.Y >= target.Y;
}

void UWidgetSlateWrapperBase::UpdateSizeBoxBoundsIfMeshDataMarkedDirty(){
    if(polygonMap.IsValid()){
        //is tested.
        if (polygonMap->BoundsUpdated())
        {
            FVector2D bound = polygonMap->Bounds();
            bound += FVector2D(margin);
            SetWidthAndHeightSizeBox(bound);
        }
    }
    
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


bool UWidgetSlateWrapperBase::GetResolutionIsValid(FVector2D &outRes){
    if(polygonMap.IsValid()){
        //get res
        //outRes = polygonMap->Bounds();
        //return true;
    }

    if(MySlateWidget.IsValid()){
        FGeometry Geometry = MySlateWidget->GetCachedGeometry();
        FVector2D LocalSize = Geometry.GetLocalSize();
        outRes = LocalSize;
        return true;
    }
    return false;
}

FVector2D UWidgetSlateWrapperBase::GetResolution(){
    FVector2D outres;
    if(GetResolutionIsValid(outres)){
        return outres;
    }
    return FVector2D(-1, -1);
}

void UWidgetSlateWrapperBase::SetResolution(FVector2D scale){
    
    //the scaling task system is still needed in tick - for reasons i dont know this
    //method apparently can be called before PostInitProperties.
    if(bWasConstructed && polygonMap.IsValid()){
        task.Update(scale);
        /*UiDebugHelper::logMessage(
            FString::Printf(
                TEXT("UWidgetSlateWrapperBase::SetResolution, debug lifecycle (%d) polygonmap valid and constructed %s"),
                id,
                *scale.ToString()
            )
        );*/
    }
    else
    {
        taskRawXY.Update(scale);
    }


    //debug
    //ProcessScalingTasks();
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
    
}

void UWidgetSlateWrapperBase::SetResolutionYUniform(int scale){
    if(polygonMap.IsValid()){
        if(bWasConstructed){
            FVector2D bounds = polygonMap->Bounds();
            if(bounds.Y <= 0.0f){
                return;
            }
            float a = scale / bounds.Y;
            bounds *= a;
            SetResolution(bounds);
            return;
        }
    }
    //taskRawY.Update(FVector2D(0,scale));
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

    if(false){
        FString msg = FString::Printf(
            TEXT("UWidgetSlateWrapperBase scale override (%.2f %.2f)"),
            size.X, size.Y
        );
        UiDebugHelper::logMessage(msg);
    }
    
}

///Temporary reference! - use one at a time!
SlateMeshDataPolygon &UWidgetSlateWrapperBase::FindFromMap(int layer){
    InitSharedPolygonMapPtrIfNeeded();
    return polygonMap->FindPolygonByLayerInternal(layer);
}