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

    totalBoneLengthCopy = completeDistance;

    float half = completeDistance / 2.0f;
    hip.setTranslation(0, 0, 0); //from to hip is 0,0,0
    knee.setTranslation(0, 0, -half); //from hip to knee
    foot.setTranslation(0, 0, -half); //from knee to foot

    toFootTip = FVector(half / 2, 0, 0); //x is forward for now
}

/// @brief set etha to a value between 0 and 1, extends leg from a range from 0 to 1, 0 being fully extended
/// @param etha etha fraction, will be clamped
void BoneIk::setEtha(float etha){
    setEtha(etha, 0);
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
    float thetaFoot_pitch = lambda - legPitchThetaRadian;
   
    float thetaKnee_pitch = lambda * 2; // bein anziehen


    hip.pitchRad(thetaHip_pitch * -1); //hip to knee, flippen einmal.
    knee.pitchRad(thetaKnee_pitch); //knee to foot
    
    //new: foot rotates too to be 90 degree to ground (which is orthogonal for now)
    foot.pitchRad(thetaFoot_pitch * -1);
    
}



/// @brief sets the etha for the bone from current rotation
/// @param etha 
void BoneIk::setEthaFromCurrentRotation(float etha){
     if(etha < 0.0f){
        etha = 0.0f;
    }
    if(etha > 1.0f){
        etha = 1.0f;
    }
    
    //mit dem arcus cosinus einen cosinus wert wieder zu winkel bauen, um dann zu rotieren
    float lambda = std::acosf(1.0f - etha); // bein nach vorne, in radian!
    float thetaHip_pitch = lambda; //additional for swing
    float thetaFoot_pitch = lambda;
   
    float thetaKnee_pitch = lambda * 2; // bein anziehen
 

    hip.pitchRadAdd(thetaHip_pitch * -1); //hip to knee, flippen einmal.
    knee.pitchRadAdd(thetaKnee_pitch); //knee to foot
    
    //new: foot rotates too to be 90 degree to ground (which is orthogonal for now)
    foot.pitchRadAdd(thetaFoot_pitch * -1);
    
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



//rotate hip / shoulder
void BoneIk::rotateFirstLimbDeg(float xDeg, float yDeg, float zDeg){
    MMatrix rotator = MMatrix::createRotatorFromDeg(xDeg, yDeg, zDeg);
    hip.rotate(rotator);
}

void BoneIk::rotateFirstLimbRad(float xDeg, float yDeg, float zDeg){
    MMatrix rotator = MMatrix::createRotatorFromRad(xDeg, yDeg, zDeg);
    //hip *= rotator;
    hip.rotate(rotator);
}

//rotation of last part (hand for example)
void BoneIk::rotateLastLimbDeg(float xDeg, float yDeg, float zDeg){
    MMatrix rotator = MMatrix::createRotatorFromDeg(xDeg, yDeg, zDeg);
    //foot *= rotator;
    foot.rotate(rotator);
}

void BoneIk::rotateLastLimbRad(float xDeg, float yDeg, float zDeg){
    MMatrix rotator = MMatrix::createRotatorFromRad(xDeg, yDeg, zDeg);
    //foot *= rotator;
    foot.rotate(rotator);
}







/// @brief will rotate the complete bone (chain) towards a target location with a weight
/// @param vec vector in meters, (1,1,1) is 1 METER!
/// @param weight weight direction on knee / (ellbow) --> will only be measured in z and y
void BoneIk::rotateEndToTarget(FVector &vec, FVector &weight){
    // man muss hier das end node irgendwo hinbewegen und dann pitch und roll so wählen
    // wie als würde ein gewicht daran hängen
    // das wird gut.
    resetAllRotations();

    //etha reverse bauen
    FVector zeroVec(0, 0, 0);

    //normalize the target location if exceeding the bone lenght
    float distance = FVector::Distance(zeroVec, vec);
    if (distance > totalBoneLengthCopy)
    {
        vec = vec.GetSafeNormal();
        vec *= totalBoneLengthCopy;
    }

    //FString distPrint = FString::Printf(TEXT("debug distance %.2f"), distance);
    //DebugHelper::logMessage(distPrint); //korrekt

    //distance to etha: (remember, here: 0 is extended, 1 is fully to hip, 180 deg angle)
    float etha = 1.0f - (distance / totalBoneLengthCopy); //richtig so, etha flippen
    

    // totalBoneLengthCopy * 0.01f * distance; //to percentage fraction
    // clamp
    if(etha < 0.0f){
        etha = 0.0f;
    }
    if(etha > 1.0f){
        etha = 1.0f;
    }

    //etha = 1.0 - etha; //um richtig zu flippen? von 0.0 zu 1.0 full extended (not clear if needed here, testing needed)
    float angleFromEtha = std::acos(1.0f - etha);

    float hipAngle = angleFromEtha * -1;
    float kneeAngle = angleFromEtha * 2;

    //FString s = FString::Printf(TEXT("debug etha %.2f, debug angle %.2f"), etha, MMatrix::radToDegree(hipAngle));
    //DebugHelper::logMessage(s);

    
    //WEIGHT KNICK RICHTUNG
    //anhand des wights dann knicken flippen
    //also -x oder -z sorgen für einen invertierten knick
    if((weight.X < 0 || weight.Z < 0) && hipAngle < 0){ //hipAngle < 0 ist gegen uhrzeiger sinn scheinbar
        //flip knee / ell
        hipAngle *= -1;
        kneeAngle *= -1;
    }

    //ETHA & WEIGHT ---> funktioniert auch wie erwartet 
    
    hip.pitchRadAdd(hipAngle); 
    knee.pitchRadAdd(kneeAngle); //knee to foot
    foot.pitchRadAdd(hipAngle);
    
    
   
    //GLOBAL TO TARGET ROTATION
    float globalSideAdd = ((M_PI / 2) - pitchAngleTo(vec)) * -1; //GEPRÜFT! RICHTIG!
    // - pi/2 (90 grad) um relativ zum bein zu machen, im urhzeigersinn
    //90 grad dazu um relativ zu machen, - winkel um korrekt zu drehen den anderen winkel


    hip.pitchRadAdd(globalSideAdd);


    //FString s2 = FString::Printf(TEXT("debug angle %.2f"), globalSideAdd);
    //DebugHelper::logMessage(s2);



    //return;

    

    //return;

    
    /**
     * -- Klarstellung um nichts zu vergessen --
     * 1) yaw heisst:
     * rotation auf xy ebene von oben gesehen
     * 
     * 2) extra rotation auf yaw und roll
     *    muss theoretisch nur auf hip stattfinden
     *    weil nurnoch das gesamte bein rotiert wird und sich
     *    das end gelenk schon an der richtigen end position befindet
     *    wegen dem pitch (die höhe stimmt quasi)
     *    jetzt muss nurnoch der gesamte vektor gedreht werden, das bein als GANZES
     *    -> was ja richtig gedacht nur inital Hip Rotation ist. Easy <3
     */

    //return;

    //works as expected.

    //GOLBAL ROTATION ON YAW - correct.
    //problem mit yz flip

    float yawAngleTarget = yawAngleTo(vec);
    //float yawAngleTarget = yawAngleTo(vec); // scheint richtig zu sein rotieren zum target

    hip.yawRadAdd(yawAngleTarget); //scheint korrekt zu sein
    //DebugHelper::showScreenMessage(vec); //vec richtig, angle FALSCH
    FString s = FString::Printf(TEXT("ANGLE %.2f"), MMatrix::radToDegree(yawAngleTarget));
    DebugHelper::showScreenMessage(s);
    // knee.yawRadAdd(yawAngleTarget * -2);

    return;

    //roll causing the issues seemingly.

    //GLOBAL ROTATION ON ROLL - testing needed
    //sollte nur hip rotieren und nicht ziel punkt schrotten
    float rollAngleWeight = rollAngleTo(weight);
    //*-1;
    hip.rollRadAdd(rollAngleWeight);

    

}



// NOT IN USE!

/// @brief testing needed
/// rotates the knee towards a specified target
/// @param vec target for the knee / ellbow
void BoneIk::rotateTowardsLocalTarget(FVector &vec){

    resetAllRotations();

    float xzSideViewAngle = pitchAngleTo(vec);
    float xyTopViewAngle = yawAngleTo(vec);
    

    //wie gehabt, flippen und anwenden
    float pitchHip = xzSideViewAngle * -1;
    float pitchKnee = xzSideViewAngle * 2; //umdrehen logischer weise wie etha wie immer

    //geprüft und korrekt
   
    hip.pitchRadAdd(pitchHip);   // hip to knee, flippen einmal.
    knee.pitchRadAdd(pitchKnee); //knee to foot
    foot.pitchRadAdd(pitchHip); //new: foot rotates too to be 90 degree to ground (which is orthogonal for now)

    //return;

    //still testing needed:
    //neu: das muss yaw sein, wie 2D rotations matrix
    float yawHip = xyTopViewAngle * -1;
    float yawKnee = xyTopViewAngle * 2;
    hip.yawRadAdd(yawHip); //ONLY ADD TO HIP TO ROTATE WHOLE LEG ALONG THE AXIS (??)
    //knee.yawRadAdd(yawKnee);
    //foot.rollRadAdd(rollHip * -1); //ggf fuß garnicht drehen
    

    /**
     *  ACHTUNG UNKLAR :
     *  -> Rotation muss vorher zurück gesetzt werden ansonsten wird das nicht korrekt funktionieren
     *  -> oder? 
     *  -> UNKLAR!
     * 
     *  wird gestestet.
     */


}



/// @brief calculates the yaw angle (from top perspektive) to a local target
/// making the leg for example turn as a whole to the left or right
/// like a top view 2d rotation
/// (apply only on first joint if rotation along axis wanted)
/// @param localTarget 
/// @return 
float BoneIk::yawAngleTo(FVector &localTarget){
    //FVector forward(1, 0, 0); //sollte member var werden ggf
    FVector2D forward2d(1, 0);
    

    FVector2D xy(localTarget.X, localTarget.Y);
    xy = xy.GetSafeNormal(); //nur 2d normalisieren weil sonst fehler auftreten, nicht 3D!

    //RECHUNG:
    //cos(theta) = a dot b if a and b are normalized!
    /*
        cos(theta) = a * b | cos^-1
        theta = acos((a*b) / (|a| * |b|))

        bzw weil a und b length 1 sind:
        theta = acos((a*b) / 1)
    */

    float dot = forward2d.X * xy.X + forward2d.Y * xy.Y;

    float xyTopViewAngle = std::acosf(dot);
    //float xyTopViewAngle = std::acosf(FVector2D::DotProduct(forward2d, xy));
    // std::acosf(FVector2D::DotProduct(forward2d, xy)); //----> DOT PRODUCT IST SCHON RADIAN, KEIN ACOS!! EFFICENCY

    xyTopViewAngle *= flipRotation(forward2d.X, forward2d.Y, xy.X, xy.Y);

    return xyTopViewAngle;
}

/// @brief calculates the angle on xz pane to a local target (making the)
/// pitch rotation for example for the hip
/// @param localTarget 
/// @return 
float BoneIk::pitchAngleTo(FVector &localTarget){
    
    FVector2D forward2d(1, 0);
    forward2d = forward2d.GetSafeNormal();
    
    FVector2D xz(localTarget.X, localTarget.Z);
    xz = xz.GetSafeNormal(); //nur 2d normalisieren weil sonst fehler auftreten, nicht 3D!

    float xzSideViewAngle = std::acosf(FVector2D::DotProduct(forward2d, xz));
    //keine flip rotation hier, ist ein einzelfall
    return xzSideViewAngle;
}



/// @brief calculates the angle on yz pane to a local target (making the)
/// roll rotation for example for the hip
/// (apply only on first joint if rotation along axis wanted)
/// @param localTarget 
/// @return roll angle between forward and target
float BoneIk::rollAngleTo(FVector &localTarget){
    //FVector forward(1, 0, 0); //sollte member var werden ggf
    FVector2D forward2d(1, 0);

    FVector2D yz(localTarget.Y, localTarget.Z);
    yz = yz.GetSafeNormal(); //nur 2d normalisieren weil sonst fehler auftreten, nicht 3D!

    float xzSideViewAngle = std::acosf(FVector2D::DotProduct(forward2d, yz));

    //testing needed
    xzSideViewAngle *= flipRotation(forward2d.X, forward2d.Y, yz.X, yz.Y);


    return xzSideViewAngle;
}


/// @brief will tell if a angle between vectors is negative or not
/// @param aX 
/// @param aY 
/// @param oX 
/// @param oY 
/// @return 
int BoneIk::flipRotation(float aX, float aY, float oX, float oY){
    // Bestimmen der Drehrichtung durch das Kreuzprodukt
    float crossProduct = aX * oY - aY * oX;

    // Wenn das Kreuzprodukt negativ ist, ist der Winkel im Uhrzeigersinn,
    // also negieren wir den Winkel.
    if (crossProduct < 0) {
        return -1;
    }
    return 1;
}

/// @brief resets the rotationof hip knee and foot to original position
void BoneIk::resetAllRotations(){
    float rad = MMatrix::degToRadian(0);

    hip.resetRotation();
    knee.resetRotation();
    foot.resetRotation();

}