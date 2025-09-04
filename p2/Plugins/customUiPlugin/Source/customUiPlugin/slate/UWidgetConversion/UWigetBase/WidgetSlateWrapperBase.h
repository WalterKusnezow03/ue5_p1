#pragma once

#include "Components/Widget.h"
#include "Components/SizeBox.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"
#include "customUiPlugin/slate/MeshData2D/sharedContainer/SlatePolygonMap.h"
#include "WidgetSlateWrapperBase.generated.h"

class SSlateWidgetBase;

/// --- SLATE TO UWIDGET WRAPPER AND BASE CLASS TO CREATE ANY CUSTOM SLATE BASED WIDGETS!---
/// @brief makes the slate base usuable as a USizeBox for UMG
/// Allows direct access to the Custom Slate Mesh Data, to manipulate them.
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetSlateWrapperBase : public USizeBox
{
    GENERATED_BODY()

protected:
    bool bDebugPolygon = true;

    // --- called right after slate widget is build with a number of layers (for now 5) ---
    //Override this to init the slate widget to your liking!
    //here you can customize your mesh data for the widget, add new layers,
    //etc. 
    virtual void ConstructWidget();
    void ApplySizeAfterConstruct(); //very important to call in construct widget or Super::ConstructWidget

    void TriggerRebuildWidget();

public:
    void SetWidthAndHeight(float x, float y);

    //Tick if Slate Mesh Data is animated (External tick from UCustomUi Component Base or derived is needed!)
    //override this method but call super to listen for cursor position
    virtual void Tick(float deltatime);

    ///@brief will tell if the cursor is inside the geometry of this widget
    ///can be overriden, but super call needed!
    virtual bool dispatchClick();


    virtual void ReleaseSlateResources(bool bReleaseChildren) override;


    //rebuild function made template
    /// @brief 
    /// @tparam TSlateWidgetType 
    /// @param sharedPtr created and overriden
    /// @param layers number of mesh data layers to have
    /// @param base Sbox ref, child added, get ptr from Super::RebeuildWidget
    /// @return 
    template <typename TSlateWidgetType>
    TSharedRef<SWidget> TRebuildWidget(
        TSharedPtr<TSlateWidgetType> &sharedPtr, 
        TSharedRef<SBox> base
    ){
        //sharedPtr = SNew(TSlateWidgetType).NumLayers(layers); //hier class name vom slate einfügen
        //return sharedPtr.ToSharedRef();

        /*
        from doc:
        SBox::SetContent ( const TSharedRef< SWidget >& InContent )
        */
        
        sharedPtr = SNew(TSlateWidgetType).PolyGonMapPtr(&polygonMap); //create custom slate widget
        base->SetContent(sharedPtr.ToSharedRef()); //apply to size box
        return base;
    }

    /// MeshData from internal SSLate widget base, use in ConstructWidget() to
    /// create / manipulate the mesh data 
    SlateMeshDataPolygon *FindFromSlateWidget(int layer);

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

private:
    SlatePolygonMap polygonMap; //By value stored here.

    TSharedPtr<SSlateWidgetBase> MySlateWidget;
};