#pragma once

#include "CoreMinimal.h"
#include "IkHumanoidModell/Ik/Controller/HipController/BoneAttachment.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "CoreMath/animation/TransformInterpolator.h"
#include "CoreMath/animation/RotationInterpolator.h"
#include "CoreMath/animation/FRotatorInterpolator.h"
#include "CoreMath/animation/FVectorInterpolator.h"
#include "IkHumanoidModell/Ik/Controller/enums/ELegPhase.h"
#include "IkHumanoidModell/Ik/Controller/enums/EHipControllerStates.h"
#include "IkHumanoidModell/Ik/Controller/ControllerSetup/LocoMotionProperty/FLocomotionProperty.h"


#include "PlueckerCore/Interface/IJointInterface.h"
#include "PlueckerCore/Bone/Joint.h"
#include "PlueckerCore/Bone/RootJoint/RootJoint.h"

class FHumanoidControllerSetupPackage;

/// @brief controls two bone attachments and runnign physics based on slip data.
/// moves the underlying hip 
/// ---> this is the base class and will have static slip force handling
/// (More stable than integral precalculation)
class IKHUMANOIDMODELL_API HipController : public IJointInterface{
protected:
    bool bExtendedDebugLog = false;
    bool bDebugBlockLocomotion = false;
    bool bDebugBlockTick = false;
    void showExtendedDebugLog(float deltatime);

    void ResetAllLocomotionFlags();
    bool HipAtGroundLevel();

    virtual bool backwardsKinematicAllowed();

public:
    void EnableDebugLogExtended();

    HipController();
    virtual ~HipController();

    void setup(FHumanoidControllerSetupPackage &package);



    /// @brief resets both legs into default state.
    void ResetAndRebuild();

    /// @brief tick to update based on locomotion, gravity etc.
    /// @param deltatime 
    void Tick(float deltatime);


    //do not modify
    MMatrix &getOrientation();

    //do not modify
    MMatrix &getTranslation();


    FVector GetLocation();
    

    /// @brief places the conroller at a target, foot on ground (passed target is ground)
    void SetLocation(FVector &target);

    ///@brief overrides the translation
    void SetTranslation(FVector &target);

    // --- entity api ---
    void setStateWalking();
    void stopLocomotion();
    void stopLocomotionOnceRotationHasFinished();

    

    //api for get actors:apply damaged owner casted mesh actor
    void getActors(TArray<AActor *> &outArray);


//--- enable /disable collapse physics ---
public:
    //enable collapse physics
    virtual void SetStateCollapse(bool flag) override;

protected:

    void TickCollapsePhysics(float deltatime);
    FLocomotionProperty locomotionProperty;

    //gets the latest collision params from ASharedRaycastParamManager.
    FCollisionQueryParams GetCollisionParams();

protected:
    

    float setupLegLength = 100.0f; //some value, cant be 0 for setting location.
    float bodyMass = 30.0f; // 10 kg ?

    float motionTime = 0.3f; // 0.7f;//0.2f;

    //actor velocity / hip velocity in every direction
    FVector velocity;

    FVector latestGroundTruth;

    UWorld *worldPointer = nullptr;

    BoneAttachment legLeft;
    BoneAttachment legRight;

    ELegPhase phaseLeft;
    ELegPhase phaseRight;

    MMatrix translation;
    MMatrix orientation;

    FVector forwardDefaultLocalLocomotionFrame;
    FVector forwardRotatedLocalLocomotionFrame;

    void buildOnStart();
    void TickLocomotion(float deltatime);
    void TickFalling(float deltatime);
    void applyLocomotion(float deltatime);
    virtual void updateInterpolatorLocomotion(float deltatime);

    bool forwardMotion = true;
    bool legLeftPlaying = true;
    

    void updateInterpolatorLocomotion();
    void updateForwardTargetWorld(FVector &targetWorld);
    void updateBackwardTargetLocal(FVector &targetLocal);
    
    
    //TransformInterpolator interpolatorForwardWorld;
    //TransformInterpolator interpolatorBackwardLocal;
    FVectorInterpolator interpolatorForwardWorld;
    FVectorInterpolator interpolatorBackwardLocal;

    void setupBackwardInterpolation();
    virtual void setupBackwardInterpolation(float animationTime);
    void setupForwardInterpolation();
    void ApplyVelocityToLocalTrajectory(FVector &localTrajectory);
    //experimental
    void RemoveVelocityFromWorldTrajectory(FVector &localTrajectory);

    void drawLocation(float deltatime);

    void projectToGround(FVector &worldTarjectory);

    void applyForces(float deltatime);

    /// @brief base slip force method: to be overriden if needed
    /// @param deltatime 
    virtual void applySlipForce(float deltatime);

    void applyStancePhaseSLIPForce(float deltatime);

    /// @brief is a more simple way of applying the slip force
    /// without using a integral on backwards kinematic start.
    /// Works more stable.
    /// @param deltatime 
    void applySlipForceStatic(float deltatime);

    void applyForceGravity(float deltatime);
    void applyVelocity(float deltatime);

    void validateTransformUpdate(FVector &position);

    bool isGrounded();
    bool groundedByDistance();

    void UpdateStanceStatus();
    bool leftInStancePhase();
    bool rightInStancePhase();

    void RebuildLegsNone(float deltatime);
    void RebuildLegsEndInPlace(float deltatime);
    void RebuildLegsEndInPlaceFaceDown(float deltatime);

    void ClampMaxVelocity();

    //debug
    FVector slipAcceleration;


    //helper for anim time
    float horizontalVelocity();
    float verticalVelocity();
   

    //new rotation
public:
    void LookAt(FVector &location);
    void setupRotationForNextStep(float radianYaw);

    /// @brief current look dir from orientation
    FVector lookDirection();

    // -- debug or player control --
    void forceYawAdd(float degree);
    void forceOverrideRotation(FRotator &rotation);

protected:
    bool DEBUG_SLOWTIME = false;  // true;
    void TickHipRotation(float deltatime);
    bool anyBackwardPhase();

    //old
    //RotationInterpolator hipRotationInterpolator;

    //new - is more buggy
    FRotatorInterpolator hipRotationInterpolator;

    //if rotation set, one rotation must be finished before starting a new one
    bool rotationSet = false;
    //before ticking the rotation, this time has to be overriden
    //it can only be overriden once for a single backwards kinematic phase
    //a rotation has to be completed in a single backwards kinematic / half stance phase!
    bool rotationTimeOverriden = false; 

    bool locomotionStopRequestedOnceRotationIsFinished = false;
    FVector forwardTrajectory();
    float slowDownTimeForDebugRotationStance(float deltatime);

    //slow down velocity on rotation change, prevent slipping
    void slowDownBasedOnRotationInRadian(float radianInNextStep);
    void updateHorizontalVelocity(FVector &velocityIn);

    //locomotion state
    EHipControllerStates currentControllerState = EHipControllerStates::EIdle;
    bool locoMotionStateEnabled();

    ///@brief removes the Y sliding from a vector in world space
    ///by moving it into local space, keeps forward motion.
    void RemoveSlidingFromVector(FVector &acceleration);



    // --- debug tools ---
    float debugIntervall = 1.0f;
    float debugIntegratedTime = 0.0f;

    bool DebugHorizontalVelocityOvershoot();

    bool IsInGround();
    void DebugIsInGround();
    
    FVector GetVelocity();
    FVector2D GetHorizontalVelocity();


    // --- pluecker joints ---
public:
    virtual Joint *GetTopJoint() override;
    virtual void ReactToDamage(const FCustomHitResult &hitResult) override;

protected:
    RootJoint rootJoint;
    void SetupPlueckerJoint(UWorld *world);
    void SetPlueckerHipConstraint(BoneAttachment &attachment);
    void UpdatePlueckerJointRotation();
    void SetRotation(const MMatrix &mat);

    void UpdateRootJointOnCollapse();



    //ik carried item hands interface
    float updatedMotionTime = 1.0f;
    bool bMotionTimeUpdatedForArms = false;
    void FlagUpdatedMotionTimeForArmAnimation(float time);

public:
    //ik carried item hands interface
    bool HasMotionTimeUpdate(float &outMotionTime);
};