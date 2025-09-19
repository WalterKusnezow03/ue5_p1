#pragma once

#include "CoreMinimal.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"
#include "CoreMath/animation/FRotatorInterpolator.h"

#include "WidgetArrowBase.generated.h"

/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.
/// tick still needed to listen for cursor position and animations!
UCLASS()
class CUSTOMUIPLUGIN_API UWidgetArrowBase : public UWidgetSlateWrapperBase{
    GENERATED_BODY()

protected:
    //call super!
    //Custom Constructor override - is called internally from post properties override!
    virtual void ConstructWidget() override;
    void CreateMeshOnConstruct();
    void SetupBackground();

public:
    void AddOrRepalceState(int number, float rotationDeg);

    //overriden but super call needed
    virtual void Tick(float deltatime) override;

    virtual bool dispatchClick() override;

    void SwitchToNextRotation();
    void SwitchToNextRotation(float timeOfAnimation);
    void SwitchToNextRotationImmidiate();
    void SetAnimationTime(float time);

    void AddState(float rotationDeg);

protected:
    void ValidateTime(float &time);
    FRotator MakeRotator(float deg);
    FRotator NextStateRotator();

    SlateMeshDataPolygon &Polygon();

    MMatrix2D MakeRotationMatrixAroundCenter(
        FVector2D sizeBounds,
        FRotator &rotation
    );

private:
    const int arrowLayer = 1;
    const int backgroundLayer = 0;

    float animationTimeSet = 0.0f;
    int currentState = 0;

    TArray<float> rotationStates;

    FRotatorInterpolator interpolator;

    float NextState();

    void SetDefaultRotation();
};