#include "WidgetSlateWrapperBase.h"
#include "customUiPlugin/slate/base/SlateWidgetBase.h"


void UWidgetSlateWrapperBase::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MySlateWidget.Reset(); //sets ptr to nullptr
}

TSharedRef<SWidget> UWidgetSlateWrapperBase::RebuildWidget()
{
    return TRebuildWidget<SSlateWidgetBase>(MySlateWidget);
    
    // MySlateWidget = SNew(SSlateWidgetBase); //hier class name vom slate einfügen
    // return MySlateWidget.ToSharedRef();
}



void UWidgetSlateWrapperBase::Tick(float deltatime){
    if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
        ptr->Tick(deltatime);
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
}


/// MeshData from internal SSLate widget base, marked dirty automatically - expected that the data is modified, if 
/// getting data is sucessfull (not nullptr)
SlateMeshDataPolygon *UWidgetSlateWrapperBase::FindFromSlateWidget(int layer){
    if(SSlateWidgetBase *ptr = MySlateWidget.Get()){
        SlateMeshDataPolygon *ptrFound = &ptr->FindPolygonByLayerInternal(layer);
        //not sure if dirty mark is needed, drawn every frame anyway.
        return ptrFound;
    }
    return nullptr;
}