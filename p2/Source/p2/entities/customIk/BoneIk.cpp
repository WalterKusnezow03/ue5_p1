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
    hip.setTranslation(0, 0, 0);
    knee.setTranslation(0, 0, -half);
    foot.setTranslation(0, 0, -half);

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

    float lambda = std::acosf(1.0f - etha); //bein nach vorne, in radian!
    float thetaHip_pitch = lambda;
    float thetaKnee_pitch = lambda * 2; // bein anziehen
    //DebugHelper::showScreenMessage("lambda ", thetaKnee_pitch);

    //From Hip down to Knee!! Remember!
    hip.pitchRad(thetaHip_pitch * -1); //hip to knee
    knee.pitchRad(thetaKnee_pitch);  //knee to foot
    //fromKnee.pitchRad(thetaKnee_pitch);
}

/**
 * 
 * BUILDING
 * 
 */
void BoneIk::getData(std::vector<MMatrix*> &dataout, FVector &outVector){ //pass by reference!! nicht vergessen!
    
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
        result *= *matrizen[i];
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

    //apply offset
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
            DebugHelper::showLineBetween(world, resultDraw[i - 1], resultDraw[i], c, displayTime);
        }
    }
}




/**
 * 
 * TICK
 * 
 */

/// @brief tick the bone and rebuild drawing
/// @param world world to draw in
/// @param offset offset to have for bones
/// @param etha etha between 0 and 1
/// @param displayTime display time (f.ex. delta time)
void BoneIk::tickAndBuild(UWorld *world, FVector &offset, float etha, float displayTime){
    //remember (x roll, y pitch, z yaw)
    
    //update pitch of knee
    setEtha(etha); //should be dafault
    //setEtha(etha); //testin


    
    // fromKnee.pitch(etha * 360.0f); //test

    DebugHelper::showScreenMessage("etha ", etha);
    //DebugHelper::showScreenMessage(fromHip.asString());

    build(world, offset, FColor::Black, displayTime);

}
