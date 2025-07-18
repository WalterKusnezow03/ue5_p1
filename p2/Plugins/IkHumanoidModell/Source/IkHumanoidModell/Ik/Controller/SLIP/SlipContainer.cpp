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

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("slip integral setup trajectory: (%.2f, %.2f, %.2f) size (%2.f)"),
            featherCurrent.X, featherCurrent.Y, featherCurrent.Z, featherCurrent.Size()
        )
    );

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
    forceRaw.Z = std::abs(forceRaw.Z);

    if(bLogEnabled){
        FString message = FString::Printf(
            TEXT("--- slip force result: (%.2f %.2f %.2f), trajectory (%.2f, %.2f, %.2f) move(%.2f %.2f)"),
            forceRaw.X,
            forceRaw.Y,
            forceRaw.Z,
            legdir_featherCurrent.X,
            legdir_featherCurrent.Y,
            legdir_featherCurrent.Z,
            movedir.X,
            movedir.Y
        );
        DebugHelper::logMessage(message);
    }
    

    return forceRaw;
}

void SlipContainer::hackSigns(FVector &force, FVector &forward){
    force.X = std::abs(force.X) * (forward.X > 0 ? 1.0f : -1.0f);
    force.Y = std::abs(force.Y) * (forward.Y > 0 ? 1.0f : -1.0f);

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

    //wenn bein oben: keine kraft
    if(!slipForceAllowedDotProductUp(legNormal)){
        return 0.0f;
    }


    //new added to allow rotated trajectories
    //might not be needed at all, bricks some stuff!
    deRotateDirectionsForSlipSine(legNormal, moveNormalized);
    moveNormalized = moveNormalized.GetSafeNormal();
    legNormal = legNormal.GetSafeNormal();




    float dotProduct = FVector::DotProduct(moveNormalized, legNormal);

    /*
    CASES:
    $$
    maxForceAngle = sin(\frac{\pi}{4}) //\textit{45 grad}
    $$
    $$
    scalar = 
    \begin{cases}
    0 & \textit{ bei } dotproduct_{up} > 0 \\
    0 & \textit{ bei } dotproduct_{forward} > 0 \\
    0 & \textit{ bei } dotproduct_{forward} < maxForceAngle \\
    |\sin(\theta)| & \textit{ bei } maxForceAngle < dotproduct_{forward} < 0 \\
    \end{cases}
    $$
    
    */

    
    //wenn < 45 grad hinten: keine kraft
    float dotneg45 = -0.7f;
    if(dotProduct < dotneg45){
        return 0.0f;
    }

    //wenn bein vorne, also dot > 0: kein kraft abschnitt
    if(dotProduct >= 0.0f){ //0.0f
        return 0.0f;
    }

    //hier sin?
    float theta = std::acosf(dotProduct);

    float thetaMaxAt45 = theta * 2.0f;//1.0 bei 45 grad

    /*
    Achtung hier teilweise vorzeichen noch falsch gedreht,
    unklar wieso!

    float sign = dotProduct > 0.0f ? -1.0f : 1.0f;
    float sinScale = std::sinf(thetaMaxAt45) * sign; */

    //nur positive kraft, kein nachgeben der kraft
    //wenn nur negative winkel bertrachtet (siehe dot product bedingung)
    float sinScale = std::abs(std::sinf(thetaMaxAt45));

    if(bLogEnabled){
        FString message = FString::Printf(
            TEXT("--- slip: projected: theta: %.2f , sin scale: %.2f, feather(%.2f, %.2f,%.2f) move_xyz(%.2f,%.2f,%.2f) dot(%.2f)"),
            (MMatrix::radToDegree(theta) - 90.0f),
            sinScale,
            legNormal.X,
            legNormal.Y,
            legNormal.Z,
            moveNormalized.X,
            moveNormalized.Y,
            moveNormalized.Z,
            dotProduct
        );
        DebugHelper::logMessage(message);
    }




    return sinScale;
}


/// @brief wenn bein nach oben zeigt, keine kraft!
/// @param leg 
/// @return 
bool SlipContainer::slipForceAllowedDotProductUp(FVector &leg){
    float dotProductUp = FVector::DotProduct(FVector(0.0f, 0.0f, 1.0f), leg);

    //wenn bein oben: keine kraft
    if(dotProductUp >= 0.0f){
        return false;
    }
    return true;
}

void SlipContainer::deRotateDirectionsForSlipSine(
    FVector &legDir,
    FVector &forward
){
    FVector legCopy = legDir;

    //axis align both
    MMatrix r;
    r.setRotation(forward);
    MMatrix r1 = r.transposedRotation();
    legDir = r1 * legDir;
    forward = r1 * forward;






    /*
    //align trajectory on negative axis
    FVector leg2D(legDir.X, legDir.Y, 0);
    FVector forwardFlipped(forward.X, forward.Y, 0);

    leg2D = leg2D.GetSafeNormal();
    forwardFlipped = forwardFlipped.GetSafeNormal();

    forwardFlipped *= -1.0f;

    float dot = FVector::DotProduct(forwardFlipped, leg2D);
    if(dot > 0.0f){
        float angle = std::acosf(dot);
        float sign = forwardFlipped.X * leg2D.Y - forwardFlipped.Y * leg2D.X > 0.0f ? 1.0f : -1.0f;
        MMatrix alginOnAxis;
        alginOnAxis.yawRadAdd(MMatrix::degToRadian(angle * sign * -1.0f));

        legDir = alginOnAxis * legDir;
    }
    */

    //align trajectory on negative axis
    FVector leg2D(legDir.X, legDir.Y, 0);
    FVector forward2D(forward.X, forward.Y, 0);
    
    float dot = FVector::DotProduct(forward2D, leg2D); //direction of leg forward or backward
    float sign = dot > 0.0f ? 1.0f : -1.0f;

    //cheap projection
    legDir.X = leg2D.Size() * sign;
    legDir.Y = 0.0f;

    //works as expected
    
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("Slip: Projected legOld(%.2f %.2f %.2f) leg project x(%.2f %.2f %.2f) and forward(%.2f, %.2f)"),
            legCopy.X, legCopy.Y, legCopy.Z,
            legDir.X, legDir.Y, legDir.Z,
            forward.X, forward.Y
        )
    );
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

        DebugHelper::logMessage(
            FString::Printf(
                TEXT("slip overcome integral a: (%.2f, %.2f, %.2f) size (%2.f)"),
                a.X, a.Y, a.Z, a.Size()
            )
        );
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("slip overcome integral b: (%.2f, %.2f, %.2f) size (%2.f)"),
                b.X, b.Y, b.Z, b.Size()
            )
        );

    }
    

    FVector outforce(0, 0, 0);
    deltaTime = std::abs(deltaTime);
    time = std::abs(time);
    FVector boneCurrentEnd(0, 0, 0);
    for (float i = 0; i <= time; i += deltaTime)
    {
        float scalar = i / time; // distTarget / distAll
        
        //if(bLogEnabled) DebugHelper::logMessage(FString::Printf(TEXT("slip scalar: %.2f"), scalar));

        boneCurrentEnd = a + scalar * (b - a);
        outforce += forceUnscaled(boneCurrentEnd, moveDir);

        DebugHelper::logMessage(
            FString::Printf(
                TEXT("slip integral interpolated trajectory: (%.2f, %.2f, %.2f) size (%2.f)"),
                boneCurrentEnd.X, boneCurrentEnd.Y, boneCurrentEnd.Z, boneCurrentEnd.Size()
            )
        );
    }
    return outforce;
}


bool SlipContainer::debugSkipToCloseframes(FVector a, FVector b){
    //debug
    a.Z = 0.0f;
    b.Z = 0.0f;
    if (FVector::Dist(a, b) < 10)
    {
        DebugHelper::logMessage("Slip overCome DebugSkip!");
        return true;
    }
    return false;
}

//new: compensation velocity
void SlipContainer::setupInterpolatedD(
    FVector &endA, //start
    FVector &endB, //lift off
    FVector &movedir, 
    float time, 
    float velocityDown,
    float mass
){
    if(debugSkipToCloseframes(endA, endB)){
        return;
    }

    showEstimationVersusRealVelocity(); //prev auswertung
    DebugHelper::logMessage("Slip overcome: ----- log end ----");
    DebugHelper::logMessage("Slip overcome: ----- log start ----");

    A = endA;
    B = endB;
    time = std::abs(time);
    timeForInterpolation = time;
    
    //hack.
    if(false && velocityDown > 0.0f){
        //is up
        velocityDown = 0.0f;
    }

    

    float gravityIntegrated = -981 * time; //accumulated gravity acceleration
    float velocityWithGravityIntegrated = velocityDown + gravityIntegrated;

    //extra gravity to have 
    float vMinOptional = estimateVmin(time); //quasi normal kraft ?


    /**
     * was false! 
     */
    if(false){
        //debug
        //vMinOptional = 0.0f;

        vMinOptional = velocityWithGravityIntegrated * -1.0f;

        vMinOptional = 0.0f;
    }

    //D >= \frac{\frac{(vmin - v_0 - at) \cdot m}{t}}{F_{raw}(t)}
    //upperfrac = \frac{(- v_0 - at) \cdot m}{t}
    float upperFrac = ((vMinOptional - gravityIntegrated) * mass) / time;
    float lowerFracIntegralZ = 0.0f;

    float dtStep = time / 200.0f; //160 bei tick für time, war 1000
    FVector lowerFracIntegratedForce = forceIntegrated(
        time,
        dtStep,
        movedir,
        endA,
        endB
    );
    lowerFracIntegralZ = avoidDivisionByZero(lowerFracIntegratedForce.Z);


    float minD = upperFrac / lowerFracIntegralZ;
    validateD(minD); //validates if needed
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
    //DebugHelper::logMessage("Slip overcome: RESULT V ESTIMATED: ", resultVelocity);
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
    float a = 981.0f;

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
    if(!slipForceAllowedDotProductUp(featherCurrent)){
        return FVector(0, 0, 0);
    }
    

    FVector force = Dcurrent * forceUnscaled(featherCurrent, movedir);

    //F = m * a
    //a = F / m
    FVector a = force / mass;

    //debug
    slipVelocityIntegratedInternal += a * deltatime;

    //debug
    if(bLogEnabled){
        
        FString message = FString::Printf(
            TEXT("--- slip output: D(%.2f) force(%.2f %.2f %.2f) acceleration(%.2f %.2f %.2f), trajectory(%.2f, %.2f, %2.f) forward(%.2f, %.2f, %.2f)"
            ),
            Dcurrent,
            force.X,
            force.Y,
            force.Z,
            a.X,
            a.Y,
            a.Z,
            featherCurrent.X,
            featherCurrent.Y,
            featherCurrent.Z,
            movedir.X,
            movedir.Y,
            movedir.Z
        );
        DebugHelper::logMessage(message);
    }

    if(force.Size() > 0.0f){
        DebugHelper::showScreenMessage("positive slip!", FColor::Green);
    }

    return a;
}




void SlipContainer::showEstimationVersusRealVelocity(){
    DebugHelper::logMessage("Slip overcome RESULT -------");
    DebugHelper::logMessage("Slip overcome RESULT V ESTIMATED: ", slipVelocityEstimatedInternal);
    DebugHelper::logMessage("Slip overcome RESULT V REAL: ", slipVelocityIntegratedInternal);
    DebugHelper::logMessage("Slip overcome RESULT -------");

    slipVelocityEstimatedInternal = FVector(0, 0, 0);
    slipVelocityIntegratedInternal = FVector(0, 0, 0);
}








/// ---- Default D calculation for clamping ----


/// @brief calculates a Default D value to prevent to high values of numerical or trajectory issues
/// which are not as expected
/// @param defaultBoneSize 
/// @param mass 
/// @param motionTime 
void SlipContainer::initDefaultForParameterD(
    float defaultBoneSize,
    float mass,
    float motionTime
){
    //dont setup twicce
    if(bDLimitSetup){
        return;
    }

    defaultBoneSize = std::abs(defaultBoneSize);
    FVector trajectoryNone(0, 0, -1.0f * std::abs(defaultBoneSize));

    MMatrix rotateFront;
    MMatrix rotateBack;

    rotateFront.pitchRadAdd(MMatrix::degToRadian(-45));
    rotateBack.pitchRadAdd(MMatrix::degToRadian(45));

    FVector moveDir(1, 0, 0);

    FVector trajectoryFront = rotateFront * trajectoryNone;
    FVector trajectoryLiftOff = rotateBack * trajectoryNone;
    float velocityDown = motionTime * -981.0f;

    //project to ground
    trajectoryLiftOff.Z = -1.0f * defaultBoneSize; //abs -1

    float DCurrentCopy = Dcurrent;

    setupInterpolatedD(
        trajectoryFront,   // start
        trajectoryLiftOff, // lift off
        moveDir,
        motionTime,
        velocityDown,
        mass
    );

    //copy calculated D
    DLimit = Dcurrent;
    Dcurrent = DCurrentCopy; //reset

    //mark setup flag
    bDLimitSetup = true;


    DebugHelper::logMessage("Slip preclamped front: ", trajectoryFront);
    DebugHelper::logMessage("Slip preclamped end: ", trajectoryLiftOff);
    DebugHelper::logMessage("Slip preclamped D: ", DLimit);
    //Achtung: ist bei 12: offensichtlich falsch! 

}

/// @brief validate D if setup
/// @param dIn 
/// @return 
void SlipContainer::validateD(float &dIn){
    if(bDLimitSetup){
        float allowedIncrease = 2.0f;
        float limitLower = std::abs(DLimit) * -1.0f * allowedIncrease;
        float limitHigher = std::abs(DLimit) * allowedIncrease;

        dIn = std::max(dIn, limitLower);
        dIn = std::min(dIn, limitHigher);
    }
}