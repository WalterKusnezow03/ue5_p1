#pragma once

#include "Components/Widget.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"
#include "WidgetSlateWrapperBase.generated.h"

class SSlateWidgetBase;

/// --- SLATE TO UWIDGET WRAPPER AND BASE CLASS TO CREATE ANY SLATE BASED WIDGETS!---
/// @brief makes the slate base usuable as a UWidget for UMG
/// Allows direct access to the Custom Slate Mesh Data, to manipulate them.
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetSlateWrapperBase : public UWidget
{
    GENERATED_BODY()

public: 
    //post constructor
    void PostInitProperties() override {
        ConstructWidget();
        Super::PostInitProperties(); //DO NOT REMOVE! - causes crash :-)
    };

    //Override this to init the slate widget to your liking!
    virtual void ConstructWidget();

    //Tick if Slate Mesh Data is animated (External tick from UCustomUi Component Base or derived is needed!)
    //override this method but call super to listen for cursor position
    virtual void Tick(float deltatime);

    ///@brief will tell if the cursor is inside the geometry of this widget
    ///can be overriden, but super call needed!
    virtual bool dispatchClick();


    virtual void ReleaseSlateResources(bool bReleaseChildren) override;


    //rebuild function made template
    template <typename TSlateWidgetType>
    TSharedRef<TSlateWidgetType> TRebuildWidget(TSharedPtr<TSlateWidgetType> &sharedPtr){
        sharedPtr = SNew(TSlateWidgetType); //hier class name vom slate einfügen
        return sharedPtr.ToSharedRef();
    }

    /// MeshData from internal SSLate widget base, marked dirty automatically if 
    /// getting data is sucessfull (not nullptr)
    SlateMeshDataPolygon *FindFromSlateWidget(int layer);

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

private:
    TSharedPtr<SSlateWidgetBase> MySlateWidget;
};