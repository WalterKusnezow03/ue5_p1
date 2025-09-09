#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/baseInterface/BaseUiInterface.h"


#include "WorldMarker.generated.h"

class UWorldMarkerCanvas;

/// @brief Will hold any UWidget or UcustomUiComponentBase inside this world marker,
/// with a position, max distance from player, used by the WorldMarkerCanvas class
/// can be constructed outside the ui and added later to have dynamic markers (for example, following aactor!)
UCLASS()
class CUSTOMUIPLUGIN_API UWorldMarker : public UObject{
    GENERATED_BODY()


protected:
    virtual void PostInitProperties() override;

    /// @brief will remove itself from parent if wanted, but the widget is NOT marked as pending kill.
    virtual void BeginDestroy() override;

public:
    //api for removing self from canvas - external removal.
    void RemoveSelfFromWorldMarkerCanvas();
    //set worldmarker canvas parent, needed for external removal of self from canvas.
    void SetMarkerCanvasPointer(UWorldMarkerCanvas *canvas);

    // --- Set widget can only be called, BEFORE being added to the canvas right now! ---
    // -> WorldMarkerCanvas wont register this change!
    // -> use this method for construction only.

    ///@brief sets the widget to be rendered, from interface
    void SetWidget(IBaseUiInterface *ptr);

    ///@brief sets the widget to be rendered, removes IBaseUiInterface reference to
    ///to be ticked.
    void SetWidget(UWidget *ptr);







    ///@brief will tick the internal IBaseUiInterface
    virtual void Tick(float deltatime);

    void SetMaxDistance(int distance);

    

    void UpdateWorldPosition(FVector &worldPosIn);

    /// @brief sets the enabled status, if not enabled, no rendering or change
    /// based on player will happen at all.
    /// @param flag 
    void SetEnabled(bool flag);

    ///@brief flags if any updates should occur, might be disabled from derived instance.
    bool IsSetEnabled();
    



    void UpdateScreenPosition(FVector2D &pos);

    const FVector &worldPositionRef();
    FVector2D &screenPositionRef();

    bool IsSame(IBaseUiInterface *ptr);
    bool IsSame(UWidget *ptr);

    IBaseUiInterface *interfacePointer();
    UWidget *widgetPointer();

    ///@brief will return if in lookdir, range AND SET VISIBILTY ACCORDINGLY
    bool IsInLookDirAndRangeOfPlayerInfo();

    ///@brief will return if in lookdir, range AND SET VISIBILTY ACCORDINGLY
    bool IsInLookDirAndRange(FVector &playerLocation, FVector &look);
    
    ///@brief will return if in lookdir AND SET VISIBILTY ACCORDINGLY
    bool IsInLookDir(FVector &playerLocation, FVector &look);

    ///@brief will return if in range AND SET VISIBILTY ACCORDINGLY
    bool IsInRange(FVector &pos);

protected:

    void SetVisible(bool flag);

    int maxDistanceSquared = 100 * 100;
    bool isEnabled = true;


    IBaseUiInterface *owningInterface = nullptr;

    //from interface or other. Ownership not taken.
    UPROPERTY()
    UWidget *widget = nullptr;

    FVector worldPosition;
    FVector2D screenPosition;


    //refernce to parent
    UPROPERTY()
    UWorldMarkerCanvas *parent = nullptr;

    /// ---- todo here: store old widgets if a widget is overriden ----
    // old must be removed from canvas, new one stored here, old tracked!



};