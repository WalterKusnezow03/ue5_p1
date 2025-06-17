#include "SlipContainer.h"


SlipContainer::SlipContainer(){

}

SlipContainer::~SlipContainer(){

}

SlipContainer::SlipContainer(SlipContainer &other){
    if(this != &other){
        *this = other;
    }
}

SlipContainer SlipContainer::operator=(SlipContainer &other){
    if(this != &other){
        featherComplete = other.featherComplete; //ausrichtung scheint eine rolle zu spielen
        featherCurrent = other.featherCurrent;
        federKonstanteD = other.federKonstanteD;
    }
    return *this;
}

void SlipContainer::setup(float defaultBoneSize, FVector &currentStartToEndEffector){
    featherComplete = defaultBoneSize;
    featherCurrent = currentStartToEndEffector;

    wasSetup = true;
}


void SlipContainer::findMinDForGravityBlock(float mass){
    float gravity = -981;
    float forceGravity = mass * gravity;

    /*
    mg <= - D * (l0 * z)/(||feathercurrent||) + D * z
    mg <= - D * ((l0 * z)/(||feathercurrent||) - z)
    D >= -1 * (mg / ((l0 * z)/(||feathercurrent||) - z))
    */
    float size = featherCurrent.Size();
    size = std::max(0.0001f, size);

    float z = featherCurrent.Z;

    float denominator = ((featherComplete * z) / size) - z;

    float sign = denominator < 0.0f ? -1.0f : 1.0f;
    denominator = std::max(0.0001f, std::abs(denominator)) * sign;

    float minD = -1.0f * (forceGravity / denominator);
    //minD *= 0.00001f;

    //DebugHelper::showScreenMessage("min D ", (float)minD);

    //federKonstanteD = minD;
}

FVector SlipContainer::force(FVector &movedir){
    if(!wasSetup){
        return FVector(0, 0, 0);
    }

    /*
    war minus

    F_x(x, y) = - D * (l_{0} * x)/(sqrt(x^2 + y^2) + D * x //forward
    
    F_y(x, y) = - D * (l_{0} * y)/(sqrt(x^2 + y^2) + D * y //up (bzw side, up im paper)

    */
    float size = featherCurrent.Size();
    size = std::max(0.0001f, size);

    float frac_X = -1.0f * (featherComplete * featherCurrent.X) / size;
    float frac_Y = -1.0f * (featherComplete * featherCurrent.Y) / size;
    float frac_Z = -1.0f * (featherComplete * featherCurrent.Z) / size;

    float add_X = federKonstanteD * featherCurrent.X;
    float add_Y = federKonstanteD * featherCurrent.Y;
    float add_Z = federKonstanteD * featherCurrent.Z;

    FVector forceRaw(
        federKonstanteD * frac_X + add_X, //war -
        federKonstanteD * frac_Y + add_Y,
        federKonstanteD * frac_Z + add_Z 
    );


    /*
    hier sollte die kraft die das bein weg drückt ausgeblendet werden
    bis das bein vertikal steht
    
    */

    FVector moveNormalized = movedir.GetSafeNormal();
    FVector legNormal = featherCurrent.GetSafeNormal();

    float dotProduct = FVector::DotProduct(moveNormalized, legNormal);


    //hier sin?
    float theta = std::acosf(dotProduct);

    float thetaMaxAt45 = theta / 2.0f;//1.0 bei 45 grad
    float sinScale = std::sinf(theta);
    sinScale *= -1.0f; //negieren um beim negativen winkel auftrieb zu erzeugen

    forceRaw *= sinScale;

    DebugHelper::logMessage("--- slip info ---");
    DebugHelper::logMessage("slip theta to forward: ", MMatrix::radToDegree(theta));
    DebugHelper::logMessage("slip theta to down: ", MMatrix::radToDegree(theta) - 90);
    DebugHelper::logMessage("slip sin: ", (float)sinScale);
    DebugHelper::logMessage("slip force: ", forceRaw);


    //forceRaw *= (-1.0f * dotProduct);

    return forceRaw;
}


FVector SlipContainer::acceleration(float mass, FVector &movedir){
    //F = m * a
    //a = F / m

    //kleiner test
    findMinDForGravityBlock(mass);

    //UNKLAR
    //achtung: mass muss mal 100 genommen werden für korrekte accelertation.
    //weil unreal in cm arbeitet
    //mass *= 100.0f;
    mass = std::max(0.0001f, mass);

    FVector f = force(movedir);
    FVector acceleration = f / mass;
    return acceleration;
}