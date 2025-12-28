#pragma once

#include "CoreMinimal.h"

//will store the limb scaling properties for the humanoid modell in one place

class IKHUMANOIDMODELL_API LimbProperties {

public:
    static void GetSizeLegs(float &a, float &b){
        a = 50.0f;
        b = 50.0f;
    }

    static void GetSizeArmLimb(float &a){
        a = 40;
    }

    static void GetTorsoProperties(float &torsoHalfWidth, float &torsoHeight){
        torsoHalfWidth = 30;
        torsoHeight = 50;
    }

    static void GetSizeFingers(float &a, float &b, float &width){
        a = 3.0f;
        b = 2.0f;
        width = 1.0f;
    }

    static void GetSizeHand(float &a, float &b, float &width){
        a = 6;
        b = 6;
        width = 3;
    }

    static void GetFingerForwardOffset(float &a){
        float sizeHandYignored;
        float ignored;
        GetSizeHand(a, sizeHandYignored, ignored);
    }

private:
    LimbProperties(){}
    ~LimbProperties(){}
};