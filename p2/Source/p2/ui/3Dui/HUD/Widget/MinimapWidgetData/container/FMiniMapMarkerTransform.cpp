#include "FMiniMapMarkerTransform.h"

FMiniMapMarkerTransform::FMiniMapMarkerTransform(){

}

FMiniMapMarkerTransform::~FMiniMapMarkerTransform(){

}

FMiniMapMarkerTransform::FMiniMapMarkerTransform(const FVector2D &posIn, float angleIn){
    SetAngle(angleIn);
    SetLocation(posIn);
}

FMiniMapMarkerTransform::FMiniMapMarkerTransform(const FMiniMapMarkerTransform &other){
    if(this != &other){
        *this = other;
    }
}

FMiniMapMarkerTransform &FMiniMapMarkerTransform::operator=(
    const FMiniMapMarkerTransform &other
){
    if(this != &other){
        angleForItem = other.angleForItem;
        locationForItem = other.locationForItem;
    }
    return *this;
}

void FMiniMapMarkerTransform::SetLocation(const FVector2D &posIn){
    locationForItem = posIn;
}

void FMiniMapMarkerTransform::SetAngle(const float angle){
    angleForItem = angle;
}

FVector2D &FMiniMapMarkerTransform::GetPositionRef(){
    return locationForItem;
}


FVector2D FMiniMapMarkerTransform::Location()const{
    return locationForItem;
}

float FMiniMapMarkerTransform::Angle()const{
    return angleForItem;
}