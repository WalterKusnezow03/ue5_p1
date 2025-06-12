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

FVector SlipContainer::force(){
    if(!wasSetup){
        return FVector(0, 0, 0);
    }

    /*

    F_x(x, y) = D * (l_{0} * x)/(sqrt(x^2 + y^2) - D * x //forward
    
    F_y(x, y) = D * (l_{0} * y)/(sqrt(x^2 + y^2) - D * y //up

    */
    float size = featherCurrent.Size();
    size = std::max(0.0001f, size);

    float frac_X = (featherComplete * featherCurrent.X) / size;
    float frac_Y = (featherComplete * featherCurrent.Y) / size;
    float frac_Z = (featherComplete * featherCurrent.Z) / size;

    float subtract_X = federKonstanteD * featherCurrent.X;
    float subtract_Y = federKonstanteD * featherCurrent.Y;
    float subtract_Z = federKonstanteD * featherCurrent.Z;

    FVector forceRaw(
        federKonstanteD * frac_X - subtract_X,
        federKonstanteD * frac_Y - subtract_Y,
        federKonstanteD * frac_Z - subtract_Z 
    );

    return forceRaw;
}


FVector SlipContainer::acceleration(float mass){
    //F = m * a
    //a = F / m

    //achtung: mass muss mal 100 genommen werden für korrekte accelertation.
    //weil unreal in cm arbeitet
    mass *= 100.0f;
    mass = std::max(0.0001f, mass);
    FVector acceleration = force() / mass;
    return acceleration;
}