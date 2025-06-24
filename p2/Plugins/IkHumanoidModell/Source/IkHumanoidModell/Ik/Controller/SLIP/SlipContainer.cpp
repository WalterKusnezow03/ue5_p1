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

SlipContainer &SlipContainer::operator=(SlipContainer &other){
    if(this != &other){
        featherComplete = other.featherComplete; //ausrichtung scheint eine rolle zu spielen
        featherCurrent = other.featherCurrent;
        federKonstanteD = other.federKonstanteD;

        Dcurrent = other.Dcurrent;
        timeForInterpolation = other.timeForInterpolation;
    }
    return *this;
}

/// @brief setup for force / acceleration calculation 
/// @param defaultBoneSize 
/// @param currentStartToEndEffector 
void SlipContainer::setup(float defaultBoneSize, FVector &currentStartToEndEffector){
    featherComplete = defaultBoneSize;
    featherCurrent = currentStartToEndEffector;

    wasSetup = true;
}







/// @brief slip force unscaled, no D applied
/// @param legdir_featherCurrent 
/// @param movedir 
/// @return 
FVector SlipContainer::forceUnscaled(
    FVector &legdir_featherCurrent,
    FVector &movedir
){
    
    /*

    Full with D

    F_x(x, y) = - D * (l_{0} * x)/(sqrt(x^2 + y^2) + D * x //forward

    F_y(x, y) = - D * (l_{0} * y)/(sqrt(x^2 + y^2) + D * y //up (bzw side, up im paper)


    Without D 
    F_x(x, y) = D (-1*(l_{0} * x)/(sqrt(x^2 + y^2) + x)
    F_y(x, y) = D (-1*(l_{0} * y)/(sqrt(x^2 + y^2) + y)
    */
    float size = legdir_featherCurrent.Size();
    size = std::max(0.0001f, size);

    float frac_X = -1.0f * (featherComplete * legdir_featherCurrent.X) / size;
    float frac_Y = -1.0f * (featherComplete * legdir_featherCurrent.Y) / size;
    float frac_Z = -1.0f * (featherComplete * legdir_featherCurrent.Z) / size;

    float add_X = legdir_featherCurrent.X;
    float add_Y = legdir_featherCurrent.Y;
    float add_Z = legdir_featherCurrent.Z;

    //unscaled Force
    FVector forceRaw(
        frac_X + add_X, 
        frac_Y + add_Y,
        frac_Z + add_Z 
    );



    float sinScale = slipSineScalar(legdir_featherCurrent, movedir);
    forceRaw *= sinScale; //smooth scale force


    //hack positive force only on z
    if(forceRaw.Z < 0.0f)
        forceRaw.Z *= -1.0f;



    if(bLogEnabled){
        FString message = FString::Printf(
            TEXT("--- slip force Z result: (%.2f), trajectory (%.2f, %.2f, %.2f)"),
            forceRaw.Z,
            legdir_featherCurrent.X,
            legdir_featherCurrent.Y,
            legdir_featherCurrent.Z
        );
        DebugHelper::logMessage(message);
    }
    

    return forceRaw;
}





/// @brief smooth force with sin angle forward to leg 
/// @param legdir_featherCurrent 
/// @param movedir 
/// @return 
float SlipContainer::slipSineScalar(
    FVector &legdir_featherCurrent,
    FVector &movedir
){
    FVector moveNormalized = movedir.GetSafeNormal();
    FVector legNormal = legdir_featherCurrent.GetSafeNormal();

    if(bLogEnabled) DebugHelper::logMessage("slip move dir: ", moveNormalized);

    float dotProduct = FVector::DotProduct(moveNormalized, legNormal);


    //wenn bein vorne, also dot > 0: kein kraft abschnitt
    if(dotProduct > 0.0f){
        return 0.0f;
    }

    //hier sin?
    float theta = std::acosf(dotProduct);

    float thetaMaxAt45 = theta * 2.0f;//1.0 bei 45 grad

    /*
    Achtung hier teilweise vorzeichen noch falsch gedreht,
    unklar wieso!
    */
    float sign = dotProduct > 0.0f ? -1.0f : 1.0f; 
    float sinScale = std::sinf(thetaMaxAt45) * sign;

    //nur positive kraft, kein nachgeben der kraft
    //wenn nur negative winkel bertrachtet (siehe dot product bedingung)
    sinScale = std::abs(sinScale);

    if(bLogEnabled){
        FString message = FString::Printf(
            TEXT("--- slip theta: %.2f , sin scale: %.2f, featherX: %.2f, move_xz(%.2f,%2.f)"),
            (MMatrix::radToDegree(theta) - 90.0f),
            sinScale,
            legdir_featherCurrent.X,
            movedir.X,
            movedir.Z
        );
        DebugHelper::logMessage(message);
    }




    return sinScale;
}




/// @brief inetgrates the force from end effektor location a to b
/// @param time 
/// @param deltaTime 
/// @param moveDir 
/// @param a 
/// @param b 
/// @return 
FVector SlipContainer::forceIntegrated(
    float time, 
    float deltaTime,
    FVector &moveDir,
    FVector &a,
    FVector &b
){
    if(bLogEnabled){
        DebugHelper::logMessage("slip integral: a", a);
        DebugHelper::logMessage("slip integral: b", b);
    }
    

    FVector outforce(0, 0, 0);
    deltaTime = std::abs(deltaTime);
    time = std::abs(time);
    FVector boneCurrentEnd(0, 0, 0);
    for (float i = 0; i <= time; i += deltaTime)
    {
        float scalar = i / time; // distTarget / distAll
        if(bLogEnabled) DebugHelper::logMessage(FString::Printf(TEXT("slip scalar: %.2f"), scalar));

        boneCurrentEnd = a + scalar * (b - a);
        outforce += forceUnscaled(boneCurrentEnd, moveDir);
    }
    return outforce;
}


//new: compensation velocity
void SlipContainer::setupInterpolatedD(
    FVector &endA, //start
    FVector &endB, //lift off
    FVector &movedir, 
    float time, 
    float velocityDown,
    float defaultBoneSize,
    float mass
){
    showEstimationVersusRealVelocity();

    A = endA;
    B = endB;
    time = std::abs(time);
    timeForInterpolation = time;
    
    if(velocityDown > 0.0f){
        //is up
        velocityDown = 0.0f;
    }

    

    float gravityIntegrated = -981 * time; //accumulated gravity acceleration
    float velocityWithGravityIntegrated = velocityDown + gravityIntegrated;

    //extra gravity to have 
    float vMinOptional = estimateVmin(time); // 10.0f; //up


    //D >= \frac{\frac{(vmin - v_0 - at) \cdot m}{t}}{F_{raw}(t)}
    //upperfrac = \frac{(- v_0 - at) \cdot m}{t}
    float upperFrac = ((vMinOptional - gravityIntegrated) * mass) / time;
    float lowerFracIntegralZ = 0.0f;

    float dtStep = time / 1000.0f;
    FVector lowerFracIntegratedForce = forceIntegrated(
        time,
        dtStep,
        movedir,
        endA,
        endB
    );
    lowerFracIntegralZ = avoidDivisionByZero(lowerFracIntegratedForce.Z);


    float minD = upperFrac / lowerFracIntegralZ;
    Dcurrent = minD;
    



    DebugHelper::logMessage("Slip overcome: v gravity for timeslot (t0->t1): ", velocityWithGravityIntegrated);
    DebugHelper::logMessage("Slip overcome: v min reach (t1->t2): ", vMinOptional);
    DebugHelper::logMessage("Slip overcome: upper: ", upperFrac);
    DebugHelper::logMessage("Slip overcome: lower ForceIntegral(z): ", lowerFracIntegralZ);
    FString msg = FString::Printf(TEXT("Slip overcome: D: %.4f"), minD);
    DebugHelper::logMessage(msg);


    //check equation

    //0 <= v_0 + at +  \frac{D \cdot F_{raw}(t)}{m}t
    FVector velocityPart(0, 0, gravityIntegrated);
    FVector a = (Dcurrent * lowerFracIntegratedForce) / mass;
    FVector resultVelocity = velocityPart + a * time;

    //looks very correct
    DebugHelper::logMessage("Slip overcome: RESULT V ESTIMATED: ", resultVelocity);
    slipVelocityEstimatedInternal = resultVelocity;
}

/// @brief avoids values too close to 0
/// @param value 
/// @return 
float SlipContainer::avoidDivisionByZero(float value){
    float abs = std::abs(value);
    float sign = value >= 0.0f ? 1.0f : -1.0f;
    abs = std::max(abs, 0.01f);
    return abs * sign;
}

/// @brief min velocity to compensate from a timeslot of gravity 
/// @param timeOfAnimation 
/// @return 
float SlipContainer::estimateVmin(float timeOfAnimation){
    /*
    0 = v_0 + at
    
    v_{min} = v_0 = - at
    */
    float a = 981;

    return a * timeOfAnimation;
}


/// @brief container must be setup before !
/// @param deltatime 
/// @param mass 
/// @param movedir 
/// @return 
FVector SlipContainer::accelerationInterpolated(
    float deltatime,
    float mass,
    FVector &movedir
){
    
    FVector force = Dcurrent * forceUnscaled(featherCurrent, movedir);

    //F = m * a
    //a = F / m
    FVector a = force / mass;

    //debug
    slipVelocityIntegratedInternal += a * deltatime;

    //debug
    if((bLogEnabled || true) && featherCurrent.X < 0.0f){
        
        FString message = FString::Printf(
            TEXT("--- slip output: D(%.2f) force(%.2f %.2f %.2f) acceleration(%.2f %.2f %.2f), trajectory(%.2f, %.2f, %2.f)"),
            Dcurrent,
            force.X,
            force.Y,
            force.Z,
            a.X,
            a.Y,
            a.Z,
            featherCurrent.X,
            featherCurrent.Y,
            featherCurrent.Z
        );
        DebugHelper::logMessage(message);
    }
    
    return a;
}




void SlipContainer::showEstimationVersusRealVelocity(){
    DebugHelper::logMessage("Slip RESULT: RESULT V ESTIMATED: ", slipVelocityEstimatedInternal);
    DebugHelper::logMessage("Slip RESULT: RESULT V REAL: ", slipVelocityIntegratedInternal);

    slipVelocityEstimatedInternal = FVector(0, 0, 0);
    slipVelocityIntegratedInternal = FVector(0, 0, 0);
}