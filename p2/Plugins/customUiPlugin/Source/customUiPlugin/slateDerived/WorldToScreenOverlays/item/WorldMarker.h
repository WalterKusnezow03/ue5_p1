#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/baseInterface/BaseUiInterface.h"

#include "WorldMarker.generated.h" 

/// @brief Will hold any UWidget or UcustomUiComponentBase inside this world marker,
/// with a position, max distance from player, used by the WorldMarkerCanvas class
/// can be constructed outside the ui and added later to have dynamic markers (for example, following aactor!)
UCLASS()
class CUSTOMUIPLUGIN_API UWorldMarker : public UObject{
    GENERATED_BODY()


protected:
    virtual void PostInitProperties() override;

public:
    ///@brief will tick the internal IBaseUiInterface
    virtual void Tick(float deltatime);

    void SetMaxDistance(int distance);

    void TrySetWidget(IBaseUiInterface *ptr);
    void UpdatePosition(FVector &worldPosIn);
    void SetVisible(bool flag);

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

    int maxDistanceSquared = 100 * 100;
    bool isEnabled = true;


    IBaseUiInterface *owningInterface = nullptr;

    //from interface
    UPROPERTY()
    UWidget *widget = nullptr;

    FVector worldPosition;
    FVector2D screenPosition;
};