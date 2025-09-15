#include "HipControllerSlipIntegral.h"




/// @brief updates the interpolator for backwards kinematic
void HipControllerSlipIntegral::setupBackwardInterpolation(float dynamicMotionTime){
    
    BoneAttachment &attachment = legLeftPlaying ? legLeft : legRight;

    // ------ precalculated slip force: deprecated --------
    // NEW !
    bool bUseSecondFutureTrajectory = true;
    if(bUseSecondFutureTrajectory){
        FVector localTrajectoryNextStep = forwardTrajectory();
        ApplyVelocityToLocalTrajectory(localTrajectoryNextStep); //move trajectory to future again

        BoneAttachment &otherAttachment = legLeftPlaying ? legRight : legLeft; //same statement but flipped.

        //move next trajectory to world space and project to ground
        //transformation from hip starting joint (start effector world space)
        FVector worldTrajectoryB = otherAttachment.inWorldSpace(
            localTrajectoryNextStep, //forwardDefaultLocalLocomotionFrame,
            translation,
            orientation
        );
        projectToGround(worldTrajectoryB);
        RemoveVelocityFromWorldTrajectory(worldTrajectoryB);


        FVector fallbackForwardTrajectory = forwardRotatedLocalLocomotionFrame;

        FVector otherLegWorld = otherAttachment.endEffectorWorldLocation();
        attachment.setupSlipDataOnStanceBegin(
            orientation, // MMatrix &orientation
            translation, // MMatrix &translation
            otherLegWorld,
            worldTrajectoryB,  // FVector & nextTrajectoryOfOtherLegWorldSpace,
            dynamicMotionTime, // float time,
            verticalVelocity(),
            horizontalVelocity(),
            bodyMass,
            fallbackForwardTrajectory // used to fake scalar D if an issue happens
        );


        
    }else{
        
        //PRE CALCULATE D 
        
        //A: end effector current relative to hip
        //B: end effector lift off, when next step touches ground relative to hip -- immer der default aber rückwärts?
        //komischer hack, muss genau definiert werden!

        FVector b = forwardDefaultLocalLocomotionFrame;
        float velocityDown = verticalVelocity();
        attachment.setupSlipDataOnStanceBegin(
            orientation,
            b,                 // local end on liftoff - is flipped internally
            dynamicMotionTime, // motionTime,
            velocityDown,      // current velocity downwards to overcome
            bodyMass
        );
    }
    // ------ precalculated slip force: deprecated end --------
    
}







void HipControllerSlipIntegral::applySlipForce(float deltatime){
    applyStancePhaseSLIPForce(deltatime);
}

void HipControllerSlipIntegral::applyStancePhaseSLIPForce(float deltatime){

    //move dir for slip force
    FVector moveDir = lookDirection();

    TArray<BoneAttachment *> attachmentsToEvaluateForce;

    //try disable phase ?
    if(leftInStancePhase()) attachmentsToEvaluateForce.Add(&legLeft); 
    if(rightInStancePhase()) attachmentsToEvaluateForce.Add(&legRight);
    

    FVector velocityAdd;
    for (int i = 0; i < attachmentsToEvaluateForce.Num(); i++)
    {
        BoneAttachment *currentBoneAttachment = attachmentsToEvaluateForce[i];
        if(currentBoneAttachment){
            //if the foot is actually grounded apply force
            SlipContainer &container = currentBoneAttachment->slipData(orientation);

            FVector vTmp = container.velocityInterpolated(
                deltatime,
                bodyMass,
                moveDir
            );
            velocityAdd += vTmp;
            
        }
    }
    

    RemoveSlidingFromVector(velocityAdd);


    //velocityAdd /= share;
    velocity += velocityAdd; // shared between 2 legs.
}