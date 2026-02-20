#pragma once

#include "CoreMinimal.h"

#include "IKCarryInterfaceAxisConstraint.generated.h"

/// @brief axis constraint info for CarriedItemSocket
USTRUCT(BlueprintType)
struct IKHUMANOIDMODELL_API FIKCarryInterfaceAxisConstraint {
    GENERATED_BODY()


public:
    FIKCarryInterfaceAxisConstraint(){}

    //rotation axis restraint for carried object / local animation
    UPROPERTY(EditAnywhere, Category="AxisConstraintOption")
    bool allowRollRotation = true;

    UPROPERTY(EditAnywhere, Category="AxisConstraintOption")
    bool allowPitchRotation = true;

    UPROPERTY(EditAnywhere, Category="AxisConstraintOption")
    bool allowYawRotation = true;

    UPROPERTY(EditAnywhere, Category="AxisConstraintOption")
    bool allowLocalPositionOffset = true;
   


    void ApplyAxisConstraint(FRotator &rotation) const{
        if(!allowRollRotation){
            rotation.Roll = 0.0f;
        }
        if(!allowPitchRotation){
            rotation.Pitch = 0.0f;
        }
        if(!allowYawRotation){
            rotation.Yaw = 0.0f;
        }
    }

    void ApplyPositionConstraint(FVector &pos) const {
        if(!allowLocalPositionOffset){
            pos.X = 0.0f;
            pos.Y = 0.0f;
            pos.Z = 0.0f;
        }
    }



    void SetupNone(){
        allowRollRotation = true;
        allowPitchRotation = true;
        allowYawRotation = true; 
    }

    void SetupRollPitchYaw(bool rollIn, bool pitchIn, bool yawIn){
        allowRollRotation = rollIn;
        allowPitchRotation = pitchIn;
        allowYawRotation = yawIn;
    }



};
