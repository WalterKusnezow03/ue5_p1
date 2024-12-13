// Fill out your copyright notice in the Description page of Project Settings.

#include <algorithm>
#include "p2/entities/customIk/BoneIk.h"

BoneIk::BoneIk()
{
    hipLimbPointer = nullptr;
	kneeLimbPointer = nullptr;
	footLimbPointer = nullptr;
}

BoneIk::~BoneIk()
{
    hipLimbPointer = nullptr;
	kneeLimbPointer = nullptr;
	footLimbPointer = nullptr;
}

//offset degree start leg motion
//DEPRECATED
void BoneIk::setDegreeInital(float degree){
    deg = degree;
}


/// @brief sets up the leg or arm (Bone)
/// @param completeDistance distance in meters
void BoneIk::setupBones(float completeDistance){

    totalBoneLengthCopy = completeDistance;

    float half = completeDistance / 2.0f;
    hip.setTranslation(0, 0, 0); //from to hip is 0,0,0
    knee.setTranslation(0, 0, -half); //from hip to knee
    foot.setTranslation(0, 0, -half); //from knee to foot

    toFootTip = FVector(half / 2, 0, 0); //x is forward for now
}


/// @brief returns the current local end pos, might be renamed to currentLocalEndInterpolatedPos
/// @return 
FVector BoneIk::currentLocalFootInterpolatedPos(){
    //matrizen interpolieren um richtungsvektor zu erhalten
    std::vector<MMatrix *> matricies;
    FVector foottip;
    getMatricies(matricies, foottip);

    MMatrix result;
    for (int i = 0; i < matricies.size(); i++){
        result *= *matricies[i];
    }

    return result.getTranslation();
}

/**
 * TESTING NEEDED!
 */
FVector BoneIk::currentLocalHipInterpolatedPos(){

    FVector fromHip = currentLocalFootInterpolatedPos();
    return fromHip * -1; //es ist halt genau umgedreht, stell es dir vor!
}


/// @brief normalizes a target to the max radius (total bone length)
/// @param target 
void BoneIk::normalizeTarget(FVector &target){
    //normalize the target location if exceeding the bone lenght
    FVector zeroVec(0, 0, 0);
    float distance = FVector::Distance(zeroVec, target);
    if (distance > totalBoneLengthCopy)
    {
        target = target.GetSafeNormal();
        target *= totalBoneLengthCopy;
    }
}


/*
DEPRECATED

/// @brief set etha to a value between 0 and 1, extends leg from a range from 0 to 1, 0 being fully extended
/// @param etha etha fraction, will be clamped
void BoneIk::setEtha(float etha){
    setEtha(etha, 0);
}


//DEPRECATED -> MUST BE REFACTURED
/// @brief will set how far the leg is extended
/// @param etha value from [0,1]
/// @param legPitchThetaRadian some angle in rad initial
void BoneIk::setEtha(float etha, float legPitchThetaRadian){

    etha = clampEtha(etha);
    currentEtha = etha;
    float lambda = angleFromEtha(etha);

    float thetaHip_pitch = createHipAngle(lambda + legPitchThetaRadian);
    float thetaKnee_pitch = createKneeAngle(lambda); //lambda only sonst bend over to back
    float thetaFoot_pitch = createHipAngle(lambda - legPitchThetaRadian);

    hip.pitchRad(thetaHip_pitch); //hip to knee
    knee.pitchRad(thetaKnee_pitch); //knee to foot
    foot.pitchRad(thetaFoot_pitch); //foot just like hip
    
}
*/


float BoneIk::clampEtha(float etha){
    if(etha < 0.0f){
        etha = 0.0f;
    }
    if(etha > 1.0f){
        etha = 1.0f;
    }
    return etha;
}

/**
 * 
 ----- ETHA EXPLAINED -----

wenn das skalarprodukt zwier vektoren 0 ergibt sind sie othogonal zu einander
wenn das skalarprodukt zwier vektoren 1 ergibt sind sie paralell zu einander

wenn das bein ausgestreckt, ist der cos(theta) * n = 1 
wenn das bein ganz raungezogen ist der cos(theta) * n = 0 weil projeziert lönge 0!

wie komme ich jetzt an den winkel:
Full extended etha = 1
der acos(1) = 0,
man dreht um 0 grad
ansonsten um 90 grad
 */



/// @brief will clamp the etha value and return the angle for the hip, positive
/// @param etha etha value
/// @return angle for the hip pitch from etha as dot product 
float BoneIk::angleFromEtha(float etha){
    //mit dem arcus cosinus einen cosinus wert wieder zu winkel bauen, um dann zu rotieren
    if(etha < 0.0f){
        etha = 0.0f;
    }
    if(etha > 1.0f){
        etha = 1.0f;
    }
    
    float lambda = std::acosf(etha); //testing. Richtig! --> 1, ganz paralell, 0 grad.
    return lambda;
}


/// @brief returns the etha value for a distance
/// @param distance distance from 0,0,0 to target
/// @return etha value, dot product
float BoneIk::createEthaFromDistance(float distance){
    if(totalBoneLengthCopy == 0){
        return 0; //fully extended error
    }
    //ganz ausgestreckt: skalarprodukt = 1
    //ansonsten eingekickt. Ganz angezogen: skalarprodukt 0: 90 grad
    float etha = (distance / totalBoneLengthCopy);
    return etha;
}



/// @brief creates the hip angle and signs it autmatically
/// @param angle angle for the hip
/// @return angle signed, ready for matrix pitch apply
float BoneIk::createHipAngle(float angle){
    return angle *-1;
}
/// @brief creates the knee angle and multiplies in automatically
/// @param angle angle for the knee 
/// @return signed and multiplied angle for knee, ready for matrix pitch apply
float BoneIk::createKneeAngle(float angle){
    return angle * 2;
}


/**
 * 
 * BUILDING BONES
 * 
 */

/// @brief fills all matrix data into the array and the foot tip into the out vector
/// @param dataout output for matricies
/// @param outVector output for foot tip vektor
void BoneIk::getMatricies(std::vector<MMatrix*> &dataout, FVector &outVector){ 
    
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
    MMatrix m;
    m.setTranslation(offset);
    build(world, m, color, displayTime);
}



void BoneIk::build(UWorld *world, MMatrix &offsetAndRotation, FColor color, float displayTime){


    std::vector<MMatrix *> matrizen;
    FVector endVec;
    getMatricies(matrizen, endVec);
    build(world, offsetAndRotation, color, displayTime, matrizen);

}




void BoneIk::build(
    UWorld *world,
    MMatrix &offsetAndRotation,
    FColor color, 
    float displayTime,
    std::vector<MMatrix*> &matrizen //must not be empty
){

    // -- dirty testing --

    matrizen.insert(matrizen.begin() + 0, &offsetAndRotation);

    //std::vector<MMatrix *> matrizen;
    //matrizen.push_back(&offsetAndRotation);

    FVector endVec = toFootTip;
    // getMatricies(matrizen, endVec);
    if(matrizen.size() < 2){ // <2 weil mindestens ein knochen muss vorhanden sein
        return;
    }

    std::vector<FVector> resultDraw;

    MMatrix result = *matrizen[0];
    //resultDraw.push_back(result.getTranslation()); //ersten zeichen punkt NICHT hinzufügen, kommt aus welt koordinaten
    
    //über matrizen laufen um die vektoren zu berechnen für die zeichnung
    //beide sollen nicht mit gezeichnet werden
    for (int i = 1; i < matrizen.size(); i++)
    {
        result *= *matrizen[i]; //durch das multiplizieren der matrizen wandert man sie entlang

        resultDraw.push_back(result.getTranslation()); //translation will be now in result space always
    }

    //final vector
    FVector outputVec = result * endVec;
    resultDraw.push_back(outputVec);
    
   

    //draw
    std::vector<FColor> colors = {FColor::Green, FColor::Red, FColor::Cyan, FColor::Green};
    if (world != nullptr)
    {
        int off = 1; // nicht welt koordinaten berück sichtigen
        for (int i = off; i < resultDraw.size(); i++)
        {
            FColor c = colors[i % colors.size()];
            c = color; //override for testing
            DebugHelper::showLineBetween(world, resultDraw[i - 1], resultDraw[i], c, displayTime);
        }
    }



}





















/// @brief will rotate the complete bone (chain) towards a target location with a weight
void BoneIk::rotateEndToTarget(FVector &vec, FVector &weight){
    rotateEndToTarget(vec, weight, hip, knee, foot);
}


/// @brief is tested for forward and backward kinematics!
/// @param vec local target when x is forward
/// @param weight weight direction on middle joint limb
/// @param start start limb (foot or hip) (recommenden to be the empty starting einheits matrix)
/// @param middle middle limb joint -> ankle / ell / knee limb
/// @param end end limb foot / hand or hip / opposite of start!
void BoneIk::rotateEndToTarget(
    FVector &vec,
    FVector &weight,
    MMatrix &start,
    MMatrix &middle,
    MMatrix &end
)
{
    //TODO: add support for using the rotation which is already applied? 
    start.resetRotation();
    middle.resetRotation();
    end.resetRotation();

    /**
     * TESTING FURTHER NEEDED
     * gewicht ziegt ja irgendwo in zy pane und dann wird die bein achse (um -z) gespinnt.
     */
    //testing needed
    if(std::abs(weight.Y) >= 0.1f){ //gegen epsilon prüfen.
        float rollAngleWeight = rollAngleTo(weight);
        hip.yawRadAdd(rollAngleWeight); //yaw drehen weil fuss erstmal nach unten zeigt, rotiert um eigene achse
    }
    
    /**
     * testing x roation
     */


    //etha reverse bauen
    FVector zeroVec(0, 0, 0);

    //normalize the target location if exceeding the bone lenght
    float distance = FVector::Distance(zeroVec, vec);
    if (distance > totalBoneLengthCopy)
    {
        vec = vec.GetSafeNormal();
        vec *= totalBoneLengthCopy;
    }


    // --- KNICK BASIS ---
     
    //distance to etha: (remember, here: 0 is extended, 1 is fully to hip, 180 deg angle)
    float etha = createEthaFromDistance(distance);
    currentEtha = etha;
    float angle = angleFromEtha(etha);
    float hipAngle = createHipAngle(angle);
    float kneeAngle = createKneeAngle(angle);

   
    //WEIGHT KNICK RICHTUNG
    //anhand des wights dann knicken flippen
    //also -x oder -z sorgen für einen invertierten knick
    if(
        (weight.X < 0 || weight.Z < 0) 
        && //wenn gewicht negativ
        hipAngle < 0                      //und knick noch nach vorne (default) (insgesamt ein gegensatz)
    ){ 
        //both angles flip based on weight direction 
        hipAngle *= -1;
        kneeAngle *= -1;
    }

    //ETHA & WEIGHT ---> funktioniert auch wie erwartet 
    start.pitchRadAdd(hipAngle);   //hip nach vorne
    middle.pitchRadAdd(kneeAngle); //knee to foot
    end.pitchRadAdd(hipAngle);



    // --- KNICK BASIS ENDE ---




    // --- GLOBAL TO TARGET ROTATION ---
    
    /**
     *  --- global pitch ---
     */
    
    //warum ist das immer initial:
    //die matrix zeigt zunächst immer nach unten, so ist der knochen im konstruktor definiert
    //und so muss auch die rotation gefunden werden, egal ob vorwärts
    //oder rückwärts kinmatic! Der winkel ist beide male korrekt!
    float pitchAngle = pitchAngleToInitialLookDirOfBone(vec); 
    float globalSideAdd = createHipAngle(pitchAngle);
    start.pitchRadAdd(globalSideAdd);
    



    FString anglePrint = FString::Printf(TEXT("angle: %.1f"), MMatrix::radToDegree(globalSideAdd));
    DebugHelper::showScreenMessage(anglePrint,vec, FColor::Orange);

    /**
     * --- global yaw top view ---
     * 
     * obere perspektive, wo muss das bein hinrotiert werden von oben gesehen
     */

    // prevent bugs with Z rotation, just lock if Y not set to any direction
    bool isSideWayTarget = (std::abs(vec.Y) >= 0.1f); //statt != 0.0f ALWAYS USE EPSILON
    if (isSideWayTarget == false){
        return;
    }

    //GOLBAL ROTATION ON YAW only if Y is set!
    float yawAngleTarget = yawAngleTo(vec);
    start.yawRadAdd(yawAngleTarget);

}







/// @brief calculates the yaw angle (from top perspektive) to a local target
/// making the leg for example turn as a whole to the left or right
/// like a top view 2d rotation
/// (apply only on first joint if rotation along axis wanted)
/// @param localTarget 
/// @return signed rotation on xy pane from top
float BoneIk::yawAngleTo(FVector &localTarget){

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
/// @param localTarget target when x is forward
/// @return abs angle on xz pane, NOT Signed
float BoneIk::pitchAngleTo(FVector &localTarget){
    
    FVector2D forward2d(1, 0);
    forward2d = forward2d.GetSafeNormal();

    FVector2D xz(localTarget.X, localTarget.Z);
    xz = xz.GetSafeNormal(); //nur 2d normalisieren weil sonst fehler auftreten, nicht 3D!

    float xzSideViewAngle = std::acosf(FVector2D::DotProduct(forward2d, xz));
    //KEINE flip rotation hier, ist ein einzelfall
    return xzSideViewAngle;
}



/// @brief calculates the angle on yz pane to a local target (making the)
/// roll rotation for example for the hip
/// (apply only on first joint if rotation along axis wanted)
/// @param localTarget 
/// @return roll angle between forward and target, signed!
float BoneIk::rollAngleTo(FVector &localTarget){
    //FVector forward(1, 0, 0); //sollte member var werden ggf
    FVector2D forward2d(0, 1); //z ist up, -z ist initial 

    FVector2D yz(localTarget.Y, localTarget.Z);
    yz = yz.GetSafeNormal(); //nur 2d normalisieren weil sonst fehler auftreten, nicht 3D!

    float yzSideViewAngle = std::acosf(FVector2D::DotProduct(forward2d, yz));

    //testing needed
    yzSideViewAngle *= flipRotation(forward2d.X, forward2d.Y, yz.X, yz.Y);


    return yzSideViewAngle;
}


/// @brief will tell if a angle between vectors is negative or not (Determines the sign of angle)
/// @param aX x comp of vector a
/// @param aY y comp of vector a
/// @param oX x comp of vector b
/// @param oY y comp of vector b
/// @return +1 for counter clock wise rotation or -1. Multiply your angle with the returned
/// value to make your angle signed 
int BoneIk::flipRotation(float aX, float aY, float oX, float oY){
    //kreuzproduk 2D
    //A:=(a,b,0) B:=(c,d,0)
    //normal := A cross B
    //normal.Z = ad - bc 

    // Bestimmen der Drehrichtung durch das Kreuzprodukt
    float crossProduct_z = aX * oY - aY * oX;

    // Wenn das Kreuzprodukt negativ ist, ist der Winkel im Uhrzeigersinn,
    // also negieren wir den Winkel.
    if (crossProduct_z < 0) {
        return -1;
    }
    return 1;
}

/// @brief resets the rotationof hip knee and foot to original position
void BoneIk::resetAllRotations(){
    //float rad = MMatrix::degToRadian(0);

    hip.resetRotation();
    knee.resetRotation();
    foot.resetRotation();

}



float BoneIk::pitchAngleToInitialLookDirOfBone(FVector &localTarget){
    FVector2D forward2d(0, -1); //0, -1
    forward2d = forward2d.GetSafeNormal();

    FVector2D xz(localTarget.X, localTarget.Z);
    xz = xz.GetSafeNormal(); //nur 2d normalisieren weil sonst fehler auftreten, nicht 3D!

    
    float xzSideViewAngle = std::acosf(FVector2D::DotProduct(forward2d, xz));
    xzSideViewAngle *= flipRotation(forward2d.X, forward2d.Y, xz.X, xz.Y);

    return xzSideViewAngle;
}


float BoneIk::pitchAngleToInitiaToUpDirOfBone(FVector &localTarget){
    FVector2D forward2d(0, 1); //0, -1
    forward2d = forward2d.GetSafeNormal();

    FVector2D xz(localTarget.X, localTarget.Z);
    xz = xz.GetSafeNormal(); //nur 2d normalisieren weil sonst fehler auftreten, nicht 3D!

    
    float xzSideViewAngle = std::acosf(FVector2D::DotProduct(forward2d, xz));
    xzSideViewAngle *= flipRotation(forward2d.X, forward2d.Y, xz.X, xz.Y);

    return xzSideViewAngle;
}












/// @brief DESIGNED FOR FROM FOOT ADJUSTING HIP!
/// @param world 
/// @param vec target relative to foot
/// @param weight weight dir of angle
/// @param offsetAndRotation FOOT LOCATION WORLD (for now extended variant)
/// expects correct starting matrix where start limb is located originally (hip or grounded foot)
/// @param translationOfactor overrides translation of actor based on where hip was moved
/// @param color 
/// @param displayTime 
void BoneIk::rotateStartToTargetAndBuild( //works as expected
    UWorld*world, 
    FVector &vec, 
    FVector &weight, 
    MMatrix &offsetAndRotationOfFoot, 
    MMatrix &translationOfactor, //overrides translation of actor based on where hip was moved
    FColor color,
    float displayTime
){
    weight *= -1; //sonst falschrum, umdrehen weil rechnung umgedreht! nicht ändern, stimmt so


    std::vector<MMatrix *> matrizen;

    
    MMatrix empty; //NEW START (Hip but backwards, ersatz.)
    rotateEndToTarget(vec, weight, empty, foot, knee);
    

    matrizen.push_back(&empty); //damit first limb gezeichnet wird von foot to knee usw. Matrix multiplikation
    matrizen.push_back(&foot); 
    matrizen.push_back(&knee);
    matrizen.push_back(&hip); //damit end limb geziechnet wird


    //reverse bones
    std::vector<AActor *> bonesAttached;
    bonesAttached.push_back(hipLimbPointer);
    bonesAttached.push_back(kneeLimbPointer);
    bonesAttached.push_back(footLimbPointer);

    MMatrix newStart = buildWithOutput(
        world,
        offsetAndRotationOfFoot,
        color,
        displayTime,
        matrizen,
        bonesAttached, //new testing needed
        false
    );

    //will probably make matrix shoot in the air.
    FVector newLocation = newStart.getTranslation();

    //Override translation of actor (hip)
    translationOfactor.setTranslation(newLocation); 

    
}













/// @brief interpolates the matricies and returns the final output
/// @param world 
/// @param offsetAndRotation 
/// @param color 
/// @param displayTime 
/// @param matrizen 
/// @return 
MMatrix BoneIk::buildWithOutput(
    UWorld *world,
    MMatrix &offsetAndRotation,
    FColor color, 
    float displayTime,
    std::vector<MMatrix*> &matrizen //must not be empty
){

    // -- dirty testing --

    matrizen.insert(matrizen.begin() + 0, &offsetAndRotation);

    

    FVector endVec = toFootTip;
    // getMatricies(matrizen, endVec);
    if(matrizen.size() < 2){ // <2 weil mindestens ein knochen muss vorhanden sein
        return offsetAndRotation;
    }

    std::vector<FVector> resultDraw;

    MMatrix result = *matrizen[0];
    //resultDraw.push_back(result.getTranslation()); //ersten zeichen punkt NICHT hinzufügen, kommt aus welt koordinaten
    
    //über matrizen laufen um die vektoren zu berechnen für die zeichnung
    //beide sollen nicht mit gezeichnet werden
    for (int i = 1; i < matrizen.size(); i++)
    {
        result *= *matrizen[i]; //durch das multiplizieren der matrizen wandert man sie entlang

        resultDraw.push_back(result.getTranslation()); //translation will be now in result space always
    }

    //final vector
    FVector outputVec = result * endVec;
    resultDraw.push_back(outputVec);



    //draw
    std::vector<FColor> colors = {FColor::Green, FColor::Red, FColor::Cyan, FColor::Green};
    if (world != nullptr)
    {
        int off = 1; // nicht welt koordinaten berück sichtigen
        for (int i = off; i < resultDraw.size(); i++)
        {
            FColor c = colors[i % colors.size()];
            c = color; //override own color from params
            DebugHelper::showLineBetween(world, resultDraw[i - 1], resultDraw[i], c, displayTime);
        }
    }

    //returns the final matrix
    return result;
}





/**
 * NEW TARGET MOVEMENT AND UPDATE FOOT MATRIX
*/

/// @brief rotate end to a target and apply new foot pos if needed!
/// @param world 
/// @param target local target of foot if x axis (1,0,0) is forward
/// @param weight weight direction to apply 
/// @param offsetAndRotation calculated transform of actor 
///                         (M = location * rotation <--read direction--)
/// @param translationOfactorFoot translation of foot of actor which will be modified! ONLY TRANSLATION!
/// @param color color to draw in
/// @param displayTime displaytime to draw
void BoneIk::rotateEndToTargetAndBuild(
    UWorld*world, 
    FVector &target, 
    FVector &weight, 
    MMatrix &offsetAndRotation, 
    MMatrix &translationOfactorFoot,
    FColor color,
    float displayTime
){
    rotateEndToTarget(target, weight); //DeltaTime);

    std::vector<MMatrix *> matrizen;
    FVector footTip;
    getMatricies(matrizen, footTip);


    //new testing
    std::vector<AActor *> limbs;
    limbs.push_back(hipLimbPointer);
    limbs.push_back(kneeLimbPointer);
    limbs.push_back(footLimbPointer);


    MMatrix newStart = buildWithOutput(
        world,
        offsetAndRotation,
        color,
        displayTime,
        matrizen,
        limbs,
        true
    );

    //will probably make matrix shoot in the air.
    FVector newLocation = newStart.getTranslation();

    //Override translation of actor (FOOT!)
    translationOfactorFoot.setTranslation(newLocation);



    
}





/// @brief PASSED LIMB REFERENCE MUST NOT EVER GET INVALIDATED!
/// @param actorToAttach 
void BoneIk::attachFirtsLimb(AActor &actorToAttach){
    if(!actorIsAlreadAttached(actorToAttach)){
        hipLimbPointer = &actorToAttach;
    }
    
}

void BoneIk::attachSecondLimb(AActor &actorToAttach){
    if(!actorIsAlreadAttached(actorToAttach)){
        kneeLimbPointer = &actorToAttach;
    }
}

void BoneIk::attachThirdLimb(AActor &actorToAttach){
    if(!actorIsAlreadAttached(actorToAttach)){
        footLimbPointer = &actorToAttach;
    }
}

bool BoneIk::actorIsAlreadAttached(AActor &actor){
    return &actor == footLimbPointer ||
           &actor == kneeLimbPointer ||
           &actor == hipLimbPointer;
}



void BoneIk::updateLimb(AActor *limb, MMatrix &matrixRotator, FVector location){
    //rotator erstellen und auf actor anwenden
    if(limb != nullptr){
        limb->SetActorLocation(location);
        limb->SetActorRotation(matrixRotator.extractRotator());
    }
}







/**
 * 
 * 
 * 
 * ----- new section with limb update ------
 * 
 * 
 */

/// @brief interpolates the matricies and returns the final output
/// @param world 
/// @param offsetAndRotation 
/// @param color 
/// @param displayTime 
/// @param matrizen 
/// @return 
MMatrix BoneIk::buildWithOutput(
    UWorld *world,
    MMatrix &offsetAndRotation,
    FColor color, 
    float displayTime,
    std::vector<MMatrix*> &matrizen, //must not be empty
    std::vector<AActor*> &attachedBones,
    bool forward
){

    // -- dirty testing --

    matrizen.insert(matrizen.begin() + 0, &offsetAndRotation);

    //std::vector<MMatrix *> matrizen;
    //matrizen.push_back(&offsetAndRotation);

    FVector endVec = toFootTip;
    // getMatricies(matrizen, endVec);
    if(matrizen.size() < 2){ // <2 weil mindestens ein knochen muss vorhanden sein
        return offsetAndRotation;
    }

    std::vector<FVector> resultDraw;

    MMatrix result = *matrizen[0];
    
    //über matrizen laufen um die vektoren zu berechnen für die zeichnung
    //beide sollen nicht mit gezeichnet werden
    for (int i = 1; i < matrizen.size(); i++)
    {
        result *= *matrizen[i]; //durch das multiplizieren der matrizen wandert man sie entlang

        resultDraw.push_back(result.getTranslation()); //translation will be now in result space always

    }

    

    //final vector (proper if forward pass)
    FVector outputVec = result * endVec;
    resultDraw.push_back(outputVec);



    //draw
    std::vector<FColor> colors = {FColor::Green, FColor::Red, FColor::Cyan, FColor::Green};
    if (world != nullptr)
    {
        int off = 1; // nicht welt koordinaten berück sichtigen
        for (int i = off; i < resultDraw.size(); i++)
        {
            FColor c = colors[i % colors.size()];
            c = color; //override own color from params
            DebugHelper::showLineBetween(world, resultDraw[i - 1], resultDraw[i], c, displayTime);
        }
    }


    //reverse before that ----> doesnt give me the result i want.
    if(!forward){
        
        std::reverse(resultDraw.begin(), resultDraw.end());
        resultDraw.erase(resultDraw.begin()); //fuss weg der nicht da ist.
        resultDraw.erase(resultDraw.begin()); //fuss weg der nicht da ist noch immer.
        resultDraw.push_back(FVector());
        
    }

    

    //UPDATE BONES
    for (int i = 1; i < resultDraw.size(); i++){

        int boneIndex = i - 1;
        FVector location = resultDraw[i - 1];
        FVector nextPoint = resultDraw[i];
        

        FVector lookDir = nextPoint - location;

        if (boneIndex < attachedBones.size()){
            AActor *currentBone = attachedBones[boneIndex];
            if(currentBone != nullptr){
                currentBone->SetActorLocation(location);
                
                //rotation anpassen wenn 
                //objekt so deisnged wurde dass der pivot am oben ende des knochens ist
                //und die länge des knochens richtung -z schaut, nach unten.
                lookDir = FVector(lookDir.Z, lookDir.Y, -lookDir.X); 
                
                // Alternativ: Rotator aus einer Richtung (Forward Vector) erstellen
                FRotator DirectionRotator = lookDir.Rotation();
                currentBone->SetActorRotation(DirectionRotator);
            }
        }
    }

    // returns the final matrix
    return result;
}
