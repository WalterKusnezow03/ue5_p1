// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/entities/customIk/BoneIk.h"

BoneIk::BoneIk()
{
}

BoneIk::~BoneIk()
{
}

//offset degree start leg motion
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



/// @brief set etha to a value between 0 and 1, extends leg from a range from 0 to 1, 0 being fully extended
/// @param etha etha fraction, will be clamped
void BoneIk::setEtha(float etha){
    setEtha(etha, 0);
}

/// @brief will set how far the leg is extended
/// @param etha value from [0,1]
/// @param legPitchThetaRadian some angle in rad
void BoneIk::setEtha(float etha, float legPitchThetaRadian){

    etha = clampEtha(etha);
    currentEtha = etha;
    float lambda = angleFromEtha(etha);

    //FString s = FString::Printf(TEXT("etha %.2f angle %d"), etha, (int) MMatrix::radToDegree(lambda));
    //DebugHelper::showScreenMessage(s, FColor::Purple);


    float thetaHip_pitch = createHipAngle(lambda + legPitchThetaRadian);
    float thetaKnee_pitch = createKneeAngle(lambda); //lambda only sonst bend over to back
    float thetaFoot_pitch = createHipAngle(lambda - legPitchThetaRadian);

    hip.pitchRad(thetaHip_pitch); //hip to knee
    knee.pitchRad(thetaKnee_pitch); //knee to foot
    foot.pitchRad(thetaFoot_pitch);
    
    
}

float BoneIk::clampEtha(float etha){
    if(etha < 0.0f){
        etha = 0.0f;
    }
    if(etha > 1.0f){
        etha = 1.0f;
    }
    return etha;
}

/// @brief will clamp the etha value and return the angle for the hip, positive
/// @param etha etha value
/// @return angle for the hip pitch
float BoneIk::angleFromEtha(float etha){
    //mit dem arcus cosinus einen cosinus wert wieder zu winkel bauen, um dann zu rotieren
    if(etha < 0.0f){
        etha = 0.0f;
    }
    if(etha > 1.0f){
        etha = 1.0f;
    }
    /*
    wenn das skalarprodukt zweier vektoren 0 ergibt sind sie orthogonal zu einander
    wenn das skalarprodukt zweier normalisierter vektoren 1 ergibt sind sie paralell
    zu einander
    Wenn mein etha input 1 ist, dann ist mein output (pi/2) -> 90 grad um den ich dann z.b. vorwärts drehe
    Wenn mein ehta input 0 ist, dann ist mein output 0 grad

    Erklärung Etha Value: 
    (0 extended , 1angezogen) -> das passiert weil 1-etha der winkel dann ist
	acos(1 - 0) = acos(1) = 0 —> nicht rotieren!
	acos(1 - 1) = acos(0) = 1 —> 90 grad -> ganz einklappen
    */


    float lambda = std::acosf(1.0f - etha);
    //float lambda = std::asinf(etha);
    return lambda;
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

/// @brief returns the etha value for a distance
/// @param distance distance from 0,0,0 to target
/// @return etha value for angles or 0 (fully extended if an issue occured)
float BoneIk::createEthaFromDistance(float distance){
    if(totalBoneLengthCopy == 0){
        return 0; //fully extended error
    }
    //distance to etha: 
    //(remember, here: 0 is extended, 1 is fully to hip, 180 deg angle)
    float etha = 1.0f - (distance / totalBoneLengthCopy); //richtig so, etha flippen
    return etha;
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








/**
 * 
 * TICK
 * 
 */


//new updated method with matrix
void BoneIk::tickAndBuild(
    UWorld *world, 
    MMatrix &offsetMatrix, //offset data in world, including rotation!
    float etha, 
    float legPitchThetaRadian,
    float displayTime,
    FColor color
){
    setEtha(etha, legPitchThetaRadian); 
    build(world, offsetMatrix, color, displayTime);
}




//NEW refacture for matrix usuage
void BoneIk::tickLegMotion(UWorld *world, float deltaTime, MMatrix &offsetMatrix, FColor color){
    float displayTime = deltaTime * 2; //better visibility of debugdrawing

    deg += degreePerSecond * deltaTime;
    //clamp
	if(deg > 360.0f){
        deg = 0; //-= 360.0f;
    }
    if(deg < 0){
        deg = 0;
    }
    float radlegLiftAngle = MMatrix::degToRadian(deg);
	float etha = abstractKinematicFunctions::legLift(radlegLiftAngle);


    //LEG SWING
    legSwingRadian += deltaTime * MMatrix::degToRadian(degreePerSecond); //synchronize for now
    if(legSwingRadian > M_PI){
		legSwingRadian = -1 * M_PI;
	}

    float pitchThetaForLegSwingRadian = abstractKinematicFunctions::legSwingPitch(legSwingRadian);
	tickAndBuild(world, offsetMatrix, etha, pitchThetaForLegSwingRadian, displayTime, color);

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
    // 1.0f - (distance / totalBoneLengthCopy); //richtig so, etha flippen
    float angle = angleFromEtha(etha);
    float hipAngle = createHipAngle(angle);
    float kneeAngle = createKneeAngle(angle);

   
    //WEIGHT KNICK RICHTUNG
    //anhand des wights dann knicken flippen
    //also -x oder -z sorgen für einen invertierten knick
    if(
        (weight.X < 0 || weight.Z < 0) && //wenn gewicht negativ
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
    float PicthAngleToXForwardAxis = pitchAngleTo(vec);
    float globalSideAdd = 0.0f;
    //winkel manuell drehen weil unsigned
    if(vec.Z < 0){
        // pi/2 (90 grad) um relativ zum bein zu machen, im urhzeigersinn
        // 90 grad dazu um relativ zu machen, -1 * winkel um korrekt zu drehen
        //globalSideAdd = ((M_PI / 2) - PicthAngleToXForwardAxis) *-1; // GEPRÜFT! RICHTIG!
        globalSideAdd = createHipAngle(((M_PI / 2) - PicthAngleToXForwardAxis)); //methode negiert automatisch
    }
    else
    {
        //globalSideAdd = ((M_PI / 2) + PicthAngleToXForwardAxis) *-1; //-1 nach wie vor wegen hip angle
        globalSideAdd = createHipAngle(((M_PI / 2) + PicthAngleToXForwardAxis)); //nach oben drehen
    }

    
    start.pitchRadAdd(globalSideAdd);


    //prevent bugs with Z rotation, just lock if Y not set to any dir
    bool isSideWayTarget = (vec.Y != 0);
    if (isSideWayTarget == false){
        return;
    }

    //GOLBAL ROTATION ON YAW only if Y is set!
    float yawAngleTarget = yawAngleTo(vec);
    start.yawRadAdd(yawAngleTarget);

    /*
    //GLOBAL ROTATION ON ROLL - testing needed - BROKEN
    //sollte nur hip rotieren und nicht ziel punkt schrotten
    float rollAngleWeight = rollAngleTo(weight);
    // *-1;
    hip.rollRadAdd(rollAngleWeight);
    knee.rollRadAdd(rollAngleWeight * -2);
    */
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
    FVector2D forward2d(1, 0);

    FVector2D yz(localTarget.Y, localTarget.Z);
    yz = yz.GetSafeNormal(); //nur 2d normalisieren weil sonst fehler auftreten, nicht 3D!

    float xzSideViewAngle = std::acosf(FVector2D::DotProduct(forward2d, yz));

    //testing needed
    xzSideViewAngle *= flipRotation(forward2d.X, forward2d.Y, yz.X, yz.Y);


    return xzSideViewAngle;
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
    MMatrix &offsetAndRotation, 
    MMatrix &translationOfactor,
    FColor color,
    float displayTime
){
    weight *= -1; //sonst falschrum, umdrehen weil rechnung umgedreht! nicht ändern, stimmt so


    std::vector<MMatrix *> matrizen;

    MMatrix &foot1 = foot;
    MMatrix &knee1 = knee;
    MMatrix &hip1 = hip;

    MMatrix empty; //NEW START (Hip but backwards)


    rotateEndToTarget(vec, weight, empty, foot1, knee1);
   

    
    matrizen.push_back(&empty); //damit first limb gezeichnet wird
    matrizen.push_back(&foot1);
    matrizen.push_back(&knee1);
    matrizen.push_back(&hip1); //damit end limb geziechnet wird (?unklar, einfach so lassen)

    MMatrix newStart = buildWithOutput(
        world,
        offsetAndRotation,
        color,
        displayTime,
        matrizen
    );

    //will probably make matrix shoot in the air.
    FVector newLocation = newStart.getTranslation();

    //Override translation of actor (hip)
    translationOfactor.setTranslation(newLocation); 

    // -- debug draw --
    //FVector oldLocation = translationOfactor.getTranslation();
    //DebugHelper::showLineBetween(world, FVector(0, 0, 0), newLocation, FColor::Yellow);
    //DebugHelper::showLineBetween(world, FVector(0, 0, 0), oldLocation, FColor::Purple);
    
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

    //std::vector<MMatrix *> matrizen;
    //matrizen.push_back(&offsetAndRotation);

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
 * NEW TARGET MOVEMENT AND UPDATE MATRIX
*/
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
    

    MMatrix newStart = buildWithOutput(
        world,
        offsetAndRotation,
        color,
        displayTime,
        matrizen
    );

    //will probably make matrix shoot in the air.
    FVector newLocation = newStart.getTranslation();

    //Override translation of actor (FOOT!)
    translationOfactorFoot.setTranslation(newLocation);

}




//new method with hip adjust on runtime
void BoneIk::rotateEndToTargetAndBuild(
    UWorld*world, 
    FVector &target, 
    FVector &weight, 
    MMatrix &offsetAndRotation, 
    MMatrix &translationOfactorFoot,
    MMatrix &translationOfactorhip, //hip adjust new
    FColor color,
    float displayTime
){
    //adjust hip based on distance, das muss im actor passieren, nicht hier!
    //UNKLAR!
    //wenn der frame projected wurde muss doch der knochen das hip nachzieehn wenn nötig
    //unklar warum das hier nicht geht / zu viel ist!
    //KÖNNTE JEDENFALLS AUCH IM ACTOR PASSIEREN!
    FVector offset;
    if(!isTragetInRange(target, offset)){
            
        //in range of one bone max
        FVector zeroVec(0, 0, 0);
        float length = std::abs(FVector::Dist(zeroVec, offset));
        if(length < totalBoneLengthCopy){

            //rotateStartToTargetAndBuild(); //DAS VIELLEICHT TESTEN!

            //translationOfactorhip += offset; //update offset for next frame
            FVector rawZ(0, 0, offset.Z);
            //translationOfactorhip += rawZ;
            translationOfactorhip += offset;
            
            DebugHelper::showLineBetween(
                world,
                offsetAndRotation.getTranslation(),
                offsetAndRotation.getTranslation() + rawZ,
                FColor::Blue
            );
        }
        
        
    }

    rotateEndToTarget(target, weight); //DeltaTime);

    std::vector<MMatrix *> matrizen;
    FVector footTip;
    getMatricies(matrizen, footTip);
    

    MMatrix newStart = buildWithOutput(
        world,
        offsetAndRotation,
        color,
        displayTime,
        matrizen
    );

    //will probably make matrix shoot in the air.
    FVector newLocation = newStart.getTranslation();

    //Override translation of actor (FOOT!)
    translationOfactorFoot.setTranslation(newLocation);

}











/**
 * 
 * NEW TESTING SECTION
 * 
 * todo:
 * target in range checken, wenn nein muss hip angepasst werden!
 * extra methode mit zu target und hip anpassung
 * 
 */

bool BoneIk::isTargetInRange(FVector &other){
    FVector zeroVec(0, 0, 0);
    float length = FVector::Dist(zeroVec, other);
    return length < totalBoneLengthCopy;
}

/// @brief direction to offset will be saved in second param if vector wasnt in range of bone
/// @param other 
/// @param fromRangeOffset 
/// @return 
bool BoneIk::isTragetInRange(FVector &other, FVector &fromRangeOffset){
    FVector zeroVec(0, 0, 0);
    float length = FVector::Dist(zeroVec, other);

    length *= 0.95f; //be friendly, allow floating point errors. DO NOT REMOVE

    bool inRange = length <= totalBoneLengthCopy;
    if(!inRange && length != 0){

        FVector normalizedTarget = other;
        normalizeTarget(normalizedTarget);

        //output copy from outer radius to target
        fromRangeOffset = other - normalizedTarget; // AB = B - A;
        return false;
    }

    return true;
}
