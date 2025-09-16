#include "WidgetArrowBase.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"

//Custom Constructor override - is called internally from post properties override!
void UWidgetArrowBase::ConstructWidget(){
    CreateMeshOnConstruct();
    SetDefaultRotation();
}


#include "Algo/Reverse.h"
void UWidgetArrowBase::CreateMeshOnConstruct(){
    SlateMeshDataPolygon &p = Polygon();
    SlateMeshData &meshData = p.MeshDataRef();


    //TODO: OFFSET TO SIDE ???? MAKE UNIFORM QUAD SHAPED BOUNDS!

    //arrow shape face right
    /*
    *--*
      *--*
        *--*   
      *--*
    *--*

    1-->2
    |   |
    0<--3
    */

    //new line based
    TArray<FVector2D> lineComplteLeft;
    lineComplteLeft.Add(FVector2D(0, 0));
        lineComplteLeft.Add(FVector2D(5, 5));
            lineComplteLeft.Add(FVector2D(10, 10));
        lineComplteLeft.Add(FVector2D(5, 15));
    lineComplteLeft.Add(FVector2D(0, 20));

    TArray<FVector2D> lineComplteRight = lineComplteLeft;
    MMatrix2D move;
    move.SetTranslation(FVector2D(5,0));
    for(FVector2D &current : lineComplteRight){
        current = move * current;
    }

    if(true){
        for (int i = 1; i < lineComplteLeft.Num(); i++){
            FVector2D &v0 = lineComplteLeft[i - 1];
            FVector2D &v1 = lineComplteRight[i - 1];
            FVector2D &v2 = lineComplteRight[i];
            FVector2D &v3 = lineComplteLeft[i];
            meshData.Append(v0,v1,v2,v3);
        }
    }
    
    //scale up to 200
    MMatrix2D globalScale;
    globalScale.Scale(1.2f);
    meshData.ApplyTransformationImmidiate(globalScale);

    int inset = 30;
    FVector2D centerInset(inset, inset);
    meshData.CenterAroundPivotImmidiate(centerInset);


    p.SetFullColor(FLinearColor::White);

    //also extend the bottom right (max max corner)
    if(polygonMap.IsValid()){
        polygonMap->SetScaleExtionsion(inset);
    }


    //Base::
    //SetWidthAndHeightToUniformBounds(); //broken right now!
}

void UWidgetArrowBase::SetDefaultRotation(){
    SetAnimationTime(0.5f); //some default time
    FRotator t1 = MakeRotator(0.0f);
    FRotator t2 = MakeRotator(0.0f);
    interpolator.setTarget(t1, t2, 0.0f);
}

FRotator UWidgetArrowBase::MakeRotator(float deg){
    FRotator r;
    r.Yaw = deg;
    return r;
}

SlateMeshDataPolygon &UWidgetArrowBase::Polygon(){
    return FindFromMap(layerNum);
}

//overriden but super call needed
void UWidgetArrowBase::Tick(float deltatime){
    Super::Tick(deltatime);

    if(!interpolator.hasReachedTarget()){
        FRotator ticked = interpolator.interpolate(deltatime);
        
        //one and only layer
        SlateMeshDataPolygon &p = Polygon();
        SlateMeshData &meshData = p.MeshDataRef();
        
        MMatrix2D M = MakeRotationMatrixAroundCenter(meshData.CenterOfMesh(), ticked);
        meshData.SetRuntimeTransformation(M);
    }
}

MMatrix2D UWidgetArrowBase::MakeRotationMatrixAroundCenter(
    FVector2D center,
    FRotator &rotator
){
    
    //move rotation to center
    MMatrix2D T;
    T.SetTranslation(center);

    //M = R * T <-- lese richtung
    MMatrix2D rotation;
    rotation.makeIdentity();
    rotation.RadAdd(MMatrix2D::degToRadian(rotator.Yaw));

    //M = T * R * T^-1 <-- lese richtung
    //go back
    MMatrix2D T1;
    T1.SetTranslation(-center);

    MMatrix2D tr = T * rotation;
    MMatrix2D M = tr * T1;
    return M;
}

/// --- Click, Tick and Animation ---
bool UWidgetArrowBase::dispatchClick(){
    if(Super::dispatchClick()){
        SwitchToNextRotation();
        return true;
    }
    return false;
}

void UWidgetArrowBase::AddState(float rotationDeg){
    rotationStates.Add(rotationDeg);
}

void UWidgetArrowBase::SwitchToNextRotation(){
    SwitchToNextRotation(animationTimeSet);
}

void UWidgetArrowBase::SwitchToNextRotation(float timeOfAnimation){
    FRotator current = interpolator.interpolate(0.0f);
    FRotator next = NextStateRotator();
    ValidateTime(timeOfAnimation);
    interpolator.setTarget(current, next, timeOfAnimation);

    if(rotationStates.Num() > 0){
        currentState = (currentState + 1) % rotationStates.Num();
    }
    
}

FRotator UWidgetArrowBase::NextStateRotator(){
    return MakeRotator(NextState());
}

float UWidgetArrowBase::NextState(){
    int next = currentState + 1;
    if(rotationStates.Num() > 0){
        int index = next % rotationStates.Num();
        return rotationStates[index];
    }
    return 0.0f;
}

void UWidgetArrowBase::ValidateTime(float &time){
    time = std::max(time, 0.0f);
}

void UWidgetArrowBase::SetAnimationTime(float time){
    ValidateTime(time);
    animationTimeSet = time;
}