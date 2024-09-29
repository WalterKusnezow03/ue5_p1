// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "skelletonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class P2_API UskelletonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    UskelletonAnimInstance();

    UFUNCTION(BlueprintCallable, Category = "IK")
    void SetLeftHandEffectorLocation(FVector NewLocation);

    UFUNCTION(BlueprintCallable, Category = "IK")
    void SetLeftElbowJointTargetLocation(FVector NewLocation);

    UFUNCTION(BlueprintCallable, Category = "IK")
    void SetRightHandEffectorLocation(FVector NewLocation);

    UFUNCTION(BlueprintCallable, Category = "IK")
    void SetRightElbowJointTargetLocation(FVector NewLocation);

protected:
    // Variables to hold the target locations for the BluePrint Anim Graph
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
    FVector LeftHandEffectorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
    FVector LeftElbowJointTargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
    FVector RightHandEffectorLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
    FVector RightElbowJointTargetLocation;
};
