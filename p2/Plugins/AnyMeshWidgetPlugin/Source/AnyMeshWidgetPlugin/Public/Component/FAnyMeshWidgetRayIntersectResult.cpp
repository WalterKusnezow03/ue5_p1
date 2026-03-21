#include "FAnyMeshWidgetRayIntersectResult.h"

FAnyMeshWidgetRayIntersectResult::FAnyMeshWidgetRayIntersectResult(){
    bResultIsValid = false;
}

FAnyMeshWidgetRayIntersectResult::FAnyMeshWidgetRayIntersectResult(
    const FAnyMeshWidgetRayIntersectResult &other
){
    if(this != &other){
        *this = other;
    }
}

FAnyMeshWidgetRayIntersectResult &FAnyMeshWidgetRayIntersectResult::operator=(
    const FAnyMeshWidgetRayIntersectResult &other
){ 
    if(this != &other){
        bResultIsValid = other.bResultIsValid;
        screenPosition = other.screenPosition;
    }
    return *this;
}

FAnyMeshWidgetRayIntersectResult::~FAnyMeshWidgetRayIntersectResult(){

}

bool FAnyMeshWidgetRayIntersectResult::IsResultValid(){
    return bResultIsValid;
}

void FAnyMeshWidgetRayIntersectResult::SetResult(FVector2D &screenPos){
    screenPosition = screenPos;
    bResultIsValid = true;
}

const FVector2D &FAnyMeshWidgetRayIntersectResult::constScreenPositionReference(){
    return screenPosition;
}