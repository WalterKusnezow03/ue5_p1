#pragma once

#include "Components/Widget.h"
#include "Components/SizeBox.h"
#include "customUiPlugin/slate/MeshData2D/SlateMeshDataPolygon.h"
#include "customUiPlugin/slate/MeshData2D/sharedContainer/SlatePolygonMap.h"
#include "customUiPlugin/baseInterface/BaseUiInterface.h"
#include "customUiPlugin/baseInterface/WidgetHelper.h"

#include "WidgetSlateWrapperBase.generated.h"

class SSlateWidgetBase;

/// --- SLATE TO UWIDGET WRAPPER AND BASE CLASS TO CREATE ANY CUSTOM SLATE BASED WIDGETS!---
/// @brief makes the slate base usuable as a USizeBox for UMG
/// Allows direct access to the Custom Slate Mesh Data, to manipulate them.
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetSlateWrapperBase : public USizeBox, public IBaseUiInterface
{
    GENERATED_BODY()

protected:
    bool bDebugPolygon = true;

    // --- called right after slate widget is build with a number of layers (for now 5) ---
    //Override this to init the slate widget to your liking!
    //here you can customize your mesh data for the widget, add new layers,
    //etc. 
    virtual void ConstructWidget();
    void UpdateSizeBoxBoundsIfMeshDataMarkedDirty();

    bool TickAllowed(){
        return bWasConstructed;
    }

private:
    bool bWasConstructed = false;

public:
    ///@brief sets the widget scale and with literally.
    void SetResolution(FVector2D scale);


    void SetWidthAndHeightToUniformBounds();
   
    void SetWidthAndHeight(FVector2D size);

    // ---- IBaseUiInterface ----

    // Tick for bounds update
    // if Slate Mesh Data is animated or changed! (External tick from UCustomUi Component Base or derived is needed!)
    // override this method but call super to listen for cursor position
    virtual void Tick(float deltatime) override;

    ///@brief will tell if the cursor is inside the geometry of this widget
    ///can be overriden, but super call needed!
    virtual bool dispatchClick() override;

    virtual void SetVisible(bool flag) override{
        WidgetHelper::SetVisible(this, flag);
    }

    virtual UWidget *baseLayoutPointer() override {
        return this;
    }

    // ---- IBaseUiInterface End ----


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
        /**
         * Important to understand:
         * Once a SWidget is created, the data cannot be modified again,
         * it may get destructed, this: TSharedPtr<TSlateWidgetType> shared pointer,
         * might CHANGE, the mesh data cannot be stored in the SWidget, but must be 
         * stored externally. The Polygon data is provided by ptr, acting as a "lightweight"
         * Copy constructor. THE MESHDATA CANNOT BE STORED INSIDE THE SWIDGET!
         */


        //sharedPtr = SNew(TSlateWidgetType).NumLayers(layers); //hier class name vom slate einfügen
        //return sharedPtr.ToSharedRef();

        /*
        from doc:
        SBox::SetContent ( const TSharedRef< SWidget >& InContent )
        */
        
        sharedPtr = SNew(TSlateWidgetType).PolyGonMapPtr(polygonMap); //create custom slate widget
        base->SetContent(sharedPtr.ToSharedRef()); //apply to size box
        return base;
    }



protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;

    
    
    
    TSharedPtr<SlatePolygonMap> polygonMap;
    //SlatePolygonMap polygonMap; //By value stored here for SSlateWidgetBase

    ///Temporary reference!!!! - use one at a time! MAP MAY GET RESIZED!
    SlateMeshDataPolygon &FindFromMap(int layer);

private:
    void InitSharedPolygonMapPtrIfNeeded();
    TSharedPtr<SSlateWidgetBase> MySlateWidget;
};