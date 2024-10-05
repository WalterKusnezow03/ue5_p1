// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/BoneIk.h"

BoneIk::BoneIk()
{
}

BoneIk::~BoneIk()
{
}


/// @brief sets up the leg, arm
/// @param completeDistance distance in meters, for now
void BoneIk::setupBones(float completeDistance){

    float half = completeDistance / 2.0f;
    hip.setTranslation(0, 0, 0); //from to hip is 0,0,0
    knee.setTranslation(0, 0, -half); //from hip to knee
    foot.setTranslation(0, 0, -half); //from knee to foot

    toFootTip = FVector(half / 2, 0, 0); //x is forward for now
}


/// @brief set etha to a value between 0 and 1, extends leg from a range from 0 to 1, 1 being fully extended
/// @param etha etha fraction, will be clamped
void BoneIk::setEtha(float etha){
    if(etha < 0.0f){
        etha = 0.0f;
    }
    if(etha > 1.0f){
        etha = 1.0f;
    }

    //mit dem arcus cosinus einen cosinus wert wieder zu winkel bauen, um dann zu rotieren
    float lambda = std::acosf(1.0f - etha); //bein nach vorne, in radian!
    float thetaHip_pitch = lambda;
    float thetaKnee_pitch = lambda * 2; // bein anziehen

    
    hip.pitchRad(thetaHip_pitch * -1); //hip to knee, im uhrzeiger sinn deshalb pos zahl
    knee.pitchRad(thetaKnee_pitch); //knee to foot
    
    //new: foot rotates too to be 90 degree to ground (which is orthogonal for now)
    foot.pitchRad(-1 * thetaHip_pitch);
}

/**
 * 
 * BUILDING BONES
 * 
 */

/// @brief fills all matrix data into the array and the foot tip into the out vector
/// @param dataout output for matricies
/// @param outVector output for foot tip vektor
void BoneIk::getData(std::vector<MMatrix*> &dataout, FVector &outVector){ 
    
    dataout.push_back(&hip);
    dataout.push_back(&knee);
    dataout.push_back(&foot);
    outVector = toFootTip; //assign to out
}

/// @brief builds the bones and draws them
/// @param world 
/// @param toBuildFromlastAnchor 
/// @param color 
void BoneIk::build(UWorld *world, FVector &offset, FColor color, float displayTime){
    
    std::vector<MMatrix *> matrizen;
    FVector endVec;
    getData(matrizen, endVec);

    if(matrizen.size() < 1){
        return;
    }

    std::vector<FVector> resultDraw;
    resultDraw.push_back(offset); //INITIAL poINT MUST BE DRAWN AT HIP!

    MMatrix result = *matrizen[0];
    resultDraw.push_back(result.getTranslation());
    for (int i = 1; i < matrizen.size(); i++){
        result *= *matrizen[i]; //durch das multiplizieren der matrizen wandert man sie entlang
        //wie als würde man vektoren addieren, aber man multipliziert halt 

        resultDraw.push_back(result.getTranslation()); //translation will be now in result space always
    }

    //final vector
    FVector outputVec = result * endVec;
    resultDraw.push_back(outputVec);

    //upscale 
    int scale = 100;
    for (int i = 1; i < resultDraw.size(); i++){
        resultDraw[i] *= scale;
    }
    
    //COPY THE FOOT MOVED AMOUNT
    int size = resultDraw.size();
    if (size > 1)
    {
        FVector &currentFootPos = resultDraw[size - 1];
        //AB = B - A
        movedDir = prevFootPos - currentFootPos;
        prevFootPos = currentFootPos;
    }



    //apply OFFSET
    for (int i = 1; i < resultDraw.size(); i++){
        resultDraw[i] += offset;
    }

    //draw
    std::vector<FColor> colors = {FColor::Green, FColor::Red, FColor::Cyan};
    if (world != nullptr)
    {
        //DebugHelper::showLine(world, resultDraw, color, displayTime);

        for (int i = 1; i < resultDraw.size(); i++){
            FColor c = colors[i % colors.size()];
            c = color; //override for testing
            DebugHelper::showLineBetween(world, resultDraw[i - 1], resultDraw[i], c, displayTime);
        }
    }
}


FVector BoneIk::movedLastTick(){
    return movedDir;
}

/**
 * 
 * TICK
 * 
 */

/// @brief tick the bone and rebuild drawing, no own motion
/// @param world world to draw in
/// @param offset offset to have for bones
/// @param etha etha between 0 and 1
/// @param displayTime display time (f.ex. delta time)
void BoneIk::tickAndBuild(UWorld *world, FVector &offset, float etha, float displayTime){
    
    //remember (x roll, y pitch, z yaw)
    //update pitch of knee
    setEtha(etha); //etha distance percent 0 to 1.0f
    build(world, offset, FColor::Red, displayTime);

}


void BoneIk::tickAndBuild(UWorld *world, FVector &offset, float displayTime){
    build(world, offset, FColor::Green, displayTime);
}




/// @brief ticks and builds the 2 bone with a given etha and pitch angle 
/// @param world 
/// @param offset 
/// @param etha 
/// @param legPitchThetaRadian 
/// @param displayTime 
/// @param color 
void BoneIk::tickAndBuild(
    UWorld *world, 
    FVector &offset, //offset data in world
    float etha, 
    float legPitchThetaRadian,
    float displayTime,
    FColor color
){
    
    //remember (x roll, y pitch, z yaw)
    //update pitch of knee
    //etha distance percent 0 to 1.0f

    setEtha(etha, legPitchThetaRadian); 
    build(world, offset, color, displayTime);

}


void BoneIk::setEtha(float etha, float legPitchThetaRadian){
    if(etha < 0.0f){
        etha = 0.0f;
    }
    if(etha > 1.0f){
        etha = 1.0f;
    }
    

    // legPitchTheta *= -1;

    //mit dem arcus cosinus einen cosinus wert wieder zu winkel bauen, um dann zu rotieren
    float lambda = std::acosf(1.0f - etha); // bein nach vorne, in radian!
    float thetaHip_pitch = lambda + legPitchThetaRadian; //additional for swing
    float thetaFoot_pitch = lambda;
    float thetaFoot_pitch_noLeg = lambda - legPitchThetaRadian;
    float thetaKnee_pitch = lambda * 2; // bein anziehen

    hip.pitchRad(thetaHip_pitch * -1); //hip to knee, flippen einmal.
    knee.pitchRad(thetaKnee_pitch); //knee to foot
    
    //new: foot rotates too to be 90 degree to ground (which is orthogonal for now)
    //foot.pitchRad(thetaFoot_pitch * -1);
    foot.pitchRad(thetaFoot_pitch_noLeg * -1);
}






/// @brief tick with automatic ak motion functions
/// @param world 
/// @param deltaTime 
/// @param offset 
/// @param color 
void BoneIk::tickMotion(UWorld *world, float deltaTime, FVector &offset, FColor color){
    float displayTime = deltaTime * 2;

    
    deg += degreePerSecond * deltaTime;
    //mod
	if(deg > 360.0f){
        deg = 0; //-= 360.0f;
    }
    float radlegLiftAngle = MMatrix::degToRadian(deg);
	float etha = abstractKinematicFunctions::legLift(radlegLiftAngle);


    //LEG SWING
    legSwingRadian += deltaTime * MMatrix::degToRadian(degreePerSecond); //synchronize for now

    if(legSwingRadian > M_PI){
		legSwingRadian = -1 * M_PI;
	}
    float tForLegSwingRadian = abstractKinematicFunctions::legSwingPitch(legSwingRadian);

    //DebugHelper::showScreenMessage("leg theta ", MMatrix::radToDegree(tForLegSwingRadian));
	tickAndBuild(world, offset, etha, tForLegSwingRadian, displayTime, color);




    //for other lag pause
    if(!halfReached && (deg >= degreePerSecond / 2)){
        halfReached = true;
    }
}



//util

float BoneIk::halfTimePhase(){
    return 0.5f; //measured in seconds, 0.5 less always?
}

bool BoneIk::halfIsReached(){
    return halfReached;
}



//rotation of starting node section

void BoneIk::roll(float degree){
    hip.roll(degree);
}

void BoneIk::pitch(float degree){
    hip.pitch(degree);
}

void BoneIk::yaw(float degree){
    hip.yaw(degree);
}