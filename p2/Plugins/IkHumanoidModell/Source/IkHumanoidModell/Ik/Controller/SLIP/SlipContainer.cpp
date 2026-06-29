#include "SlipContainer.h"


SlipContainer::SlipContainer(){
    featherComplete = 100.0f; //SOME VALUE MUST BE SETUP, OTHERWISE ALL FORMULAS ARE BROKEN!
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





FVector SlipContainer::forceUnscaled(
    FVector &legdir_featherCurrent,
    FVector &movedir,
    bool allowSlipSineScalar //check to allow slip force to back
){
    if(legdir_featherCurrent.Size() > featherComplete){
        legdir_featherCurrent = legdir_featherCurrent.GetSafeNormal() * featherComplete;
        /*DebugHelper::logMessage(
            FString::Printf(
                TEXT("slip integral interpolated trajectory resize feather!!: (%.10f, %.10f, %.10f)"),
                legdir_featherCurrent.X,
                legdir_featherCurrent.Y,
                legdir_featherCurrent.Z
            )
        );*/
    }

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

   
    float sinScale = 
        allowSlipSineScalar ? 
        slipSineScalar(legdir_featherCurrent, movedir) : 
        0.0f;
    

    forceRaw.Z *= (1.0f + sinScale); //smooth scale force
    forceRaw.X *= sinScale;
    forceRaw.Y *= sinScale;

    //hack positive force only on z
    //forceRaw.Z = std::abs(forceRaw.Z);

    if(false && bLogEnabled){
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

/// @brief slip force unscaled, no D applied
/// @param legdir_featherCurrent 
/// @param movedir 
/// @return 
FVector SlipContainer::forceUnscaled(
    FVector &legdir_featherCurrent,
    FVector &movedir
){
    return forceUnscaled(legdir_featherCurrent, movedir, true);
}

/// @brief smooth force with sin angle forward to leg 
/// @param legdir_featherCurrent 
/// @param movedir 
/// @return 
float SlipContainer::slipSineScalar(
    const FVector &legdir_featherCurrent,
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
    if(bLogEnabled){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Slip: Projected legOld(%.2f %.2f %.2f) leg project x(%.2f %.2f %.2f) and forward(%.2f, %.2f)"),
                legCopy.X, legCopy.Y, legCopy.Z,
                legDir.X, legDir.Y, legDir.Z,
                forward.X, forward.Y
            )
        );
    }
    
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
    forceCache.ClearFrames();
    forceCache.SetTime(time);

    FVector outforce = FVector::ZeroVector;
    deltaTime = std::abs(deltaTime);
    time = std::abs(time);
    FVector dir = b - a;

    for (float i = 0; i <= time; i += deltaTime)
    {
        float scalar = i / time; // distTarget / distAll
    
        FVector boneCurrentEnd = a + (scalar * dir);
        FVector foundForce = forceUnscaled(boneCurrentEnd, moveDir);
        forceCache.AddFrame(foundForce);
        outforce += foundForce;

        if(bLogEnabled){
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("slip integral interpolated trajectory: (%s) a(%s) b(%s) scalar(%.2f) time(%.2f) (%.2f feather)"),
                    *boneCurrentEnd.ToString(),
                    *a.ToString(),
                    *b.ToString(),
                    scalar,
                    time,
                    featherComplete
                )
            );
        }
        
    }


    if(bLogEnabled){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Slip Force Integrated: F(%s) A(%s) B(%s)"),
                *outforce.ToString(),
                *a.ToString(),
                *b.ToString()
            )
        );
    } 
    
    return outforce;
}

FVector SlipContainer::forceIntegrated(
    float B1, 
    float F1, 
    float B2, 
    float deltaTime,
    FVector &moveDir,
    FVector &a,
    FVector &b
){

    float time = B1 + F1 + B2;
    forceCache.ClearFrames();
    forceCache.SetTime(time);

    FVector outforce = FVector::ZeroVector;
    deltaTime = std::abs(deltaTime);
    time = std::abs(time);
    FVector dir = b - a;



    //b1 kinematic phase
    for (float i = 0; i <= B1; i += deltaTime) {

        float scalar = i / time;

        FVector boneCurrentEnd = a + (scalar * dir);
        FVector foundForce = forceUnscaled(boneCurrentEnd, moveDir);
        forceCache.AddFrame(foundForce);
        outforce += foundForce;

        //only Z, as expected
        if(bLogEnabled){
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("Slip B1 force (%s)"),
                    *foundForce.ToString()
                )
            );
        }
        
    }


    
    //f1 stance no motion phase
    for (float i = 0; i <= F1; i += deltaTime) {
        float scalar = (B1) / time;

        FVector boneCurrentEnd = a + (scalar * dir);
        FVector foundForce = forceUnscaled(boneCurrentEnd, moveDir);
        forceCache.AddFrame(foundForce);
        outforce += foundForce;

        //only Z, as expected
        if(bLogEnabled){
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("Slip F1 static force (%s)"),
                    *foundForce.ToString()
                )
            );
        }
        
    }


    //backward kinematic b2 of other leg, end in place phase
    for (float i = 0; i <= B2; i += deltaTime) {
        float scalar = ((B1 + F1) + i) / time;

        FVector boneCurrentEnd = a + (scalar * dir);
        FVector foundForce = forceUnscaled(boneCurrentEnd, moveDir);
        forceCache.AddFrame(foundForce);
        outforce += foundForce;

        //NOT only Z, as expected ??
        if(bLogEnabled){
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("Slip B2 force (%s)"),
                    *foundForce.ToString()
                )
            );
        }
        
    }

    if(bLogEnabled){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Slip Force Integrated: F(%s) A(%s) B(%s)"),
                *outforce.ToString(),
                *a.ToString(),
                *b.ToString()
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
    FVector &endA, //start LOCAL, With rotation
    FVector &endB, //lift off, Local, with rotation
    FVector &movedir, 
    float time, 
    float velocityDown,
    float mass
){
    if(debugSkipToCloseframes(endA, endB)){
        return;
    }

    if(bLogEnabled)
        DebugHelper::logMessage("Slip overcome: ----- log start ----");

    timeForInterpolation = std::abs(time);

    //hack.
    if(false && velocityDown > 0.0f){
        //is up
        velocityDown = 0.0f;
    }

    

    float gravityIntegrated = -981.0f * time; //accumulated gravity acceleration to compensate
    

    //extra gravity removal to have
    float vMinOptional = 1.0f; // 981.0f; //5cms hoch., sind 5m/s! Achtung XDD

    //D >= \frac{\frac{(vmin - v_0 - at) \cdot m}{t}}{F_{raw}(t)}
    //upperfrac = \frac{(- v_0 - at) \cdot m}{t}
    float upperFrac = ((vMinOptional - velocityDown - gravityIntegrated) * mass);
    float lowerFracIntegralZ = 0.0f;

    float dtStep = time / 400.0f; //160 bei tick für time, war 1000
    FVector lowerFracIntegratedForce = forceIntegrated(
        time,
        dtStep,
        movedir,
        endA,
        endB
    );
    //debug
    forceCache.Debug("forceCheck", lowerFracIntegratedForce);

    //---- CAUTION makes force value NOT Same!!!!!! -----
    lowerFracIntegralZ = avoidDivisionByZero(lowerFracIntegratedForce.Z); 

   


    float minD = upperFrac / (lowerFracIntegralZ * time);
    //validateD(minD); //validates if needed
    Dcurrent = minD;

    //Update Cache
    forceCache.SetScalarDAndConvertBufferToVelocity(Dcurrent, mass);

    /// ---- LOG -----
    if(bLogEnabled){
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Slip overcome: movedir: %s"),
                *movedir.ToString()
            )
        );
        DebugHelper::logMessage("Slip overcome: v gravity at t0: ", velocityDown);

        float velocityWithGravityIntegrated = velocityDown + gravityIntegrated;
        DebugHelper::logMessage("Slip overcome: v gravity for timeslot (t0->t1): ", velocityWithGravityIntegrated);
        DebugHelper::logMessage("Slip overcome: v min reach (t1->t2): ", vMinOptional);
        DebugHelper::logMessage("Slip overcome: upper: ", upperFrac);
        DebugHelper::logMessage("Slip overcome: lower ForceIntegral(z): ", lowerFracIntegralZ);
        DebugHelper::logMessage(FString::Printf(TEXT("Slip overcome: time: %.2f "), time));
        FString msg = FString::Printf(TEXT("Slip overcome: D: %.4f"), minD);
        DebugHelper::logMessage(msg);
    }
    


    //check equation

    //0 <= v_0 + at +  \frac{D \cdot F_{raw}(t)}{m} \cdot t
    FVector velocityPart(0, 0, gravityIntegrated);
    velocityPart *= 0;

    FVector a = (Dcurrent * lowerFracIntegratedForce) / mass;
    FVector resultVelocity = a * time;

    //100% correct. (?)
    if(bLogEnabled){
        DebugHelper::logMessage("Slip cache result: Check Equation Result Velocity to remove gravity + vmin: ", resultVelocity);
        DebugHelper::logMessage("Slip cache result: Check Equation Result Velocity to remove gravity + vmin in m/s: ", resultVelocity / time);
    }
    

    //looks very correct
    forceCache.Debug("VelocityCheck ",resultVelocity); 




    // Literally a hack.
    //--- DEBUG ---
    /*
    float minReach = std::abs(gravityIntegrated);
    if (resultVelocity.Z <= minReach || resultVelocity.Z > minReach)
    {
        // v * x = vTarget;
        //x = vTarget / v
        float x = minReach / resultVelocity.Z;
        forceCache.scaleZ(x);
    }
    */

    DebugHelper::logMessage("Slip overcome: ----- log end ----");
}



//new: compensation velocity
void SlipContainer::setupInterpolatedD(
    FVector &endA, //start LOCAL, With rotation
    FVector &endB, //lift off, Local, with rotation
    FVector &movedir, 
    float B1,
    float F1,
    float B2,
    float velocityDown,
    float mass
){
    if(debugSkipToCloseframes(endA, endB)){
        //DebugHelper::logMessage("Slip B1F1B2 overcome: Skipped to close frames!");
        return;
    }


    float time = B1 + F1 + B2;
    timeForInterpolation = std::abs(time);

    float gravityIntegrated = -981.0f * time; //accumulated gravity acceleration to compensate
    

    //extra gravity removal to have
    float vMinOptional = 5.0f; // 981.0f; //5cms hoch., sind 5m/s! Achtung XDD

    //D >= \frac{\frac{(vmin - v_0 - at) \cdot m}{t}}{F_{raw}(t)}
    //upperfrac = \frac{(- v_0 - at) \cdot m}{t}
    float upperFrac = ((vMinOptional - velocityDown - gravityIntegrated) * mass);
    float lowerFracIntegralZ = 0.0f;

    float dtStep = time / 400.0f; //160 bei tick für time, war 1000
    FVector lowerFracIntegratedForce = forceIntegrated(
        B1,
        F1,
        B2,
        dtStep,
        movedir,
        endA,
        endB
    );
    //debug
    //forceCache.Debug("B1F1B2 forceCheck", lowerFracIntegratedForce);

    //---- CAUTION makes force value NOT Same!!!!!! -----
    lowerFracIntegralZ = avoidDivisionByZero(lowerFracIntegratedForce.Z); 
    Dcurrent = upperFrac / (lowerFracIntegralZ * time);
    

    //Update Cache.
    forceCache.SetScalarDAndConvertBufferToVelocity(Dcurrent, mass);

    /// ---- LOG -----

    //looks very correct

    //vmin <= v_0 + at +  \frac{D \cdot F_{raw}(t)}{m} \cdot t
    //0 <= v_0 + at - vmin +  \frac{D \cdot F_{raw}(t)}{m} \cdot t
    FVector v =
        FVector(0, 0, (velocityDown + (-981.0f * time) - vMinOptional));
    FVector a_f = (Dcurrent * lowerFracIntegratedForce) / mass; //\frac{D \cdot F_{raw}(t)}{m}
    FVector a_v = a_f * time;
    FVector resultVerticalVelocity = v + a_v;
    
    
    forceCache.Debug("B1F1B2 VelocityCheck Raw Down ", v);
    //forceCache.PrintBuffer(); //debug



    
    DebugHelper::logMessage(
        FString::Printf(
            TEXT(
                "Slip B1F1B2 cache result: Check Equation v.z m/s (expects around 0.0f): %.1f"
            ),
            resultVerticalVelocity.Z
        )
    );

    

    //DebugHelper::logMessage("Slip B1F1B2 overcome: ----- log end ----");
}

/// @brief avoids values too close to 0
/// @param value 
/// @return 
float SlipContainer::avoidDivisionByZero(float value){
    float abs = std::abs(value);
    float sign = value >= 0.0f ? 1.0f : -1.0f;
    abs = std::max(abs, 0.001f);
    return abs * sign;
}




/// @brief container must be setup before !
/// @param deltatime 
/// @param mass 
/// @param movedir 
/// @return 
FVector SlipContainer::velocityInterpolated(
    float deltatime,
    float mass,
    FVector &movedir
){
    

    if(forceCache.CacheIsSetup()){
        FVector v = forceCache.Tick(deltatime);

    
        //FVector v = (force / mass); //DO NOT DO DELTA TIME HERE! SCALING INCLUDED IN D TERM!
        //* deltatime;

        FString reached = forceCache.endReached() ? TEXT("reached") : TEXT("not reached");
        DebugHelper::logMessage(
            FString::Printf(
                TEXT("Slip cache result ticked velocity(%s) terminated(%s)"),
                *v.ToString(),
                *reached
            )
        );
        return v;
    }

    if(!slipForceAllowedDotProductUp(featherCurrent)){
        return FVector(0, 0, 0);
    }
    FVector force = Dcurrent * forceUnscaled(featherCurrent, movedir);

    //F = m * a
    //a = F / m
    FVector a = force / mass;
    DebugHelper::logMessage(
        FString::Printf(
            TEXT("Slip Output velocity %s"), *a.ToString()
        )
    );
    FVector velocity = a * deltatime;

    return velocity;
}





//small debug function of static slip force
FVector SlipContainer::StaticSlipVelocity(
    FVector &moveDir,
    FVector &currentFootRelative,
    bool isGrounded,
    float velocityDown,
    float mass,
    float deltatime,
    bool isInStance
){
    if(isGrounded){
        float vMinOptional = 0.0f;
        float gravityIntegrated = -981.0f * deltatime; // accumulated gravity acceleration to compensate

        /*
        $$
        D >= 
        \frac
        {(v_{min} - v_0 - aT) \cdot m}      //upper
        {F_{raw} \cdot T}                //lower
        $$
        */
        float upperFrac = ((vMinOptional - velocityDown - gravityIntegrated) * mass);
    
        FVector force = forceUnscaled(currentFootRelative, moveDir, isInStance);
        force.Z = avoidDivisionByZero(force.Z);
        float lowerFrac = force.Z * deltatime;

        float D = upperFrac / lowerFrac;

        /*
        Force to velocity:
        $$
        F \cdot \frac{D\cdot t}{m}  >= v
        $$
        */
        float frac = (D * deltatime) / mass;
        FVector velocity = force * frac;
        return velocity;
    }
    return FVector(0, 0, 0);
}
