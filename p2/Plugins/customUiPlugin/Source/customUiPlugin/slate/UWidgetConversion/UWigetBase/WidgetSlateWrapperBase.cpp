#include "WidgetSlateWrapperBase.h"
#include "customUiPlugin/slate/base/SlateWidgetBase.h"
#include "customUiPlugin/Private/Debug/UiDebugHelper.h"



void UWidgetSlateWrapperBase::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MySlateWidget.Reset(); //sets ptr to nullptr
}

TSharedRef<SWidget> UWidgetSlateWrapperBase::RebuildWidget()
{
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
    ConstructWidget(); //do not remove this, after widget is build, data can be manipulated!
    ApplySizeAfterConstruct(); //very important
    return t;
}



void UWidgetSlateWrapperBase::Tick(float deltatime){
    if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
        ptr->Tick(deltatime); //updates cursor position
    }
    polygonMap.Tick(deltatime);
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
        polygonMap.DebugCreatePolygons();
    }
}

void UWidgetSlateWrapperBase::ApplySizeAfterConstruct(){
    FVector2D size = polygonMap.Bounds();
    SetWidthAndHeight(size.X, size.Y);
}

void UWidgetSlateWrapperBase::SetWidthAndHeight(float x, float y){
    x = std::max(std::abs(x), 1.0f);
    y = std::max(std::abs(y), 1.0f);
    SetWidthOverride(x);
    SetHeightOverride(y);
}





/// MeshData from internal SSLate widget base (from here stored tho.)
SlateMeshDataPolygon *UWidgetSlateWrapperBase::FindFromSlateWidget(int layer){
    
    return &polygonMap.FindPolygonByLayerInternal(layer);

    /*
    if(SlateWidgetBase *ptr = MySlateWidget.Get()){
        SlateMeshDataPolygon *ptrFound = &ptr->FindPolygonByLayerInternal(layer);
        //not sure if dirty mark is needed, drawn every frame anyway.
        return ptrFound;
    }
    return nullptr;*/
}