#include "NewtonSolve.h"

NewtonSolve::NewtonSolve(){
    solutionFound = false;
}

NewtonSolve::~NewtonSolve(){

}

bool NewtonSolve::SolutionWasFound(){
    return solutionFound;
}

void NewtonSolve::resetFlag(){
    solutionFound = false;
}

void NewtonSolve::markSolved(){
    solutionFound = true;
}

void NewtonSolve::logSolution(
    FVector &target, 
    FVector &w, 
    FVector &t_w, 
    float dist,
    int attempts //attempts
){
    if(logEnabled){
        FString message = FString::Printf(
            TEXT("reached until 5xm with (cm: %.2f) (w : (% .2f, % .2f, % .2f))(target(% .2f, % .2f, % .2f))(reached(% .2f, % .2f, % .2f))"),
            dist,
            w.X, w.Y, w.Z,
            target.X, target.Y, target.Z,
            t_w.X, t_w.Y, t_w.Z
        );
    
        DebugHelper::showScreenMessage(
            message,
            FColor::Green
        );
    
        DebugHelper::logMessage("found target after tries: ", attempts);
    }
}


bool NewtonSolve::solutionIsWithinRange(FVector &a, FVector &b){
    bool ok = FVector::Dist(a, b) < minDistanceSolutionOk;
    solutionFound = ok;
    return ok;
}

bool NewtonSolve::solutionIsAlreadyWithinRange(Joint &joint, FVector &target){
    MMatrix endTransformCurrent = joint.endTransform();
    FVector endEffektorLocation = endTransformCurrent.getTranslation();
    return solutionIsWithinRange(endEffektorLocation, target);
}







FVector NewtonSolve::SolveForAngularVelocity(
    Joint *joint,  //starting joint
    FVector &target, //local target
    float desiredTime
){
    resetFlag();
    if (joint == nullptr){
        return FVector(0,0,0);
    }
    if(solutionIsAlreadyWithinRange(*joint, target)){
        DebugHelper::showScreenMessage("solution in range", FColor::Cyan);
        markSolved();
        return FVector(0, 0, 0);
    }

    MMatrix zeroTransform;
    float deltaTime = desiredTime;  // 1.0f;
    FVector w(1.0f, 0.0f, 0.0f); //muss effizienter geraten werden, 
    //zb akuelle achse mit pitch yaw zur target achse yaw pitch w(0, y, z)
    w = guessAngularVelocity(*joint, target);



    int tries = 100;
    for (int i = 0; i < tries; i++){

        
        //w_tryNext = w_current - (J^-1 * t(w_current))
        //w_tryNext = w_current - b
        FVector t_w = joint->T_vonW(zeroTransform, w, deltaTime);
        if (solutionIsWithinRange(t_w, target)){
            markSolved();

            if(logEnabled){
                float dist = FVector::Dist(t_w, target);
                logSolution(target, w, t_w, dist, i);
            }

            DebugHelper::showScreenMessage("solution found", FColor::Green);

            // debug: skallieren
            w /= desiredTime;

            return w;
        }

        //peform newton Rasphor
        Matrix3x3 jacobian = computeNumericalJacobian(
            zeroTransform,
            joint,
            w,
            deltaTime
        );
        FString s = jacobian.asString();
        DebugHelper::logMessage("jacobian ", s);

        Matrix3x3 jacobianInverse = jacobian.jordanInverse();
        testInverse(jacobian, jacobianInverse);

        FVector b = jacobianInverse * t_w;

        FVector w1 = w - b;
        w = w1;
    }


    markSolved();
    DebugHelper::showScreenMessage("solution not found", FColor::Orange);
    return w.GetSafeNormal() * 100.0f;
}

void NewtonSolve::testInverse(Matrix3x3 &a, Matrix3x3 &b){
    Matrix3x3 result = a * b;
    FVector aVec(10, 0, 0);
    FVector bVec = result * aVec;
    
    if(FVector::Dist(aVec, bVec) > 1.0f){
        DebugHelper::showScreenMessage("inverse invalid", FColor::Red);
    }

    /**
     * matrizen nahe 0,0,0... zero matrix: schlecht. Idee: skallieren
     */
    FString s1 = result.asString();
    FString s2 = a.asString();
    FString s3 = b.asString();
    DebugHelper::logMessage("------");
    DebugHelper::logMessage("identity: ", s1);
    DebugHelper::logMessage("a: ", s2);
    DebugHelper::logMessage("a^-1: ", s3);
    DebugHelper::logMessage("------");
    
}

//noch sehr unklar.
Matrix3x3 NewtonSolve::computeNumericalJacobian(
    MMatrix &zeroTransform,
    Joint *joint, 
    const FVector& w, 
    float deltaTime
){
    if(joint == nullptr){
        Matrix3x3 result;
        result.makeIdentity();
        DebugHelper::showScreenMessage("joint invalid");
        return result;
    }

    /*
    float beFriendly = 10.0f; //100  -- 100000 too much
    float h = 1e-4f * beFriendly;

    h = 1e-2f;
    */
    float wScale = w.Size();
    float h = 0.01f * wScale;

    /**
     * konstruiert pseudo ableitung per vektor variable
     */
    Matrix3x3 J; // jacobian
    for (int i = 0; i < 3; i++)
    {
        /*
        per column wird abgeleitet
             tx / wx  tx / wy  tx/wz
        J =  ty / wx  ty / wy  ty/wz
             tz / wx  tz / wy  tz/wz
        
        */

        //w nach aktueller ableitung setuppen (spalten weise)
        FVector dw(0.0f, 0.0f, 0.0f);

        if (i == 0)
            dw.X = h;
        if(i == 1)
            dw.Y = h;
        if(i == 2)
            dw.Z = h;

        FVector w1 = w + dw;
        FVector w2 = w - dw;
        
        //debug um unterschied zu zeigen
        //w1 *= 100000.0f;
        //w2 /= 100000.0f;

        DebugHelper::logMessage("w1 ", w1);
        DebugHelper::logMessage("w2 ", w2);

        FVector t_w2  = joint->T_vonW(zeroTransform, w1, deltaTime); 
        FVector t_w1 = joint->T_vonW(zeroTransform, w2, deltaTime);
        //Werte liegen zu dicht bei einander

        DebugHelper::logMessage("tw_1 ", t_w1);
        DebugHelper::logMessage("tw_2 ", t_w2);

        FString s = FString::Printf(TEXT("Dist tw %.100f "), FVector::Dist(t_w2, t_w1));
        DebugHelper::logMessage(s);

        // f'(x) = dy / dx = lim(h-> 0) (f(x+h) - f(x)) / h
        FVector dT_upperPart = (t_w2 - t_w1); // dy ---> achtung ist immer 0 ?

        float OneDiv_dw_lowerPart = (1.0f / (2.0f * h)); //dx
        //ergibt sehr kleinen wert bei 1.0 / 1mio

        FVector column = dT_upperPart * OneDiv_dw_lowerPart; //dy /dx

        // Set column i of derivates found
        J.setColumn(column, i);
    }

    return J;
}

bool NewtonSolve::increaseNeeded(FVector &a, FVector &b){
    return FVector::Dist(a, b) < 0.05f;
}

/// @brief might be refactured, tried differently because not everytime a solution
/// is found
/// @param joint 
/// @param targetLocal 
/// @return 
FVector NewtonSolve::guessAngularVelocity(Joint &joint, FVector &targetLocal){
    //builded chain
    MMatrix endTransformCurrent = joint.endTransform();
    FVector endEffektorLocation = endTransformCurrent.getTranslation();


    FVector a = endEffektorLocation.GetSafeNormal();
    FVector b = targetLocal.GetSafeNormal();

    FVector axis = FVector::CrossProduct(a, b);
    float dot = FVector::DotProduct(a, b);
    dot = FMath::Clamp(dot, -1.0f, 1.0f); // wichtig!

    float angle = FMath::Acos(dot);

    axis = axis.GetSafeNormal();
    FVector angularVelocityGuess = axis * angle;
    return angularVelocityGuess;

    /*
    //winkel zwischen den achsen vermessen (yaw und pitch)
    
    //yaw:
    FVector b = targetLocal.GetSafeNormal();
    FVector a = endEffektorLocation.GetSafeNormal();

    FVector2D a2D(a.X, a.Y);
    FVector2D b2D(b.X, b.Y);

    float dot = a2D.X * b2D.X + a2D.Y * b2D.Y;
    float sign = a.X * b.Y - a.Y * b.X; //normal.Z

    //cos(theta) = a dot b //wenn a und b normalisiert sind
    //acos(a dot b) = theta
    float angleYaw = sign * std::acosf(dot);

    //derotate
    MMatrix removeYaw;
    removeYaw.yawRadAdd(-angleYaw);
    a = removeYaw * a;
    b = removeYaw * b;

    //pitch
    FVector2D a2DPitch(a.X, a.Z);
    FVector2D b2DPitch(b.X, b.Z);
    dot = a2DPitch.X * b2DPitch.X + a2DPitch.Y * b2DPitch.Y;
    sign = a2DPitch.X * b2DPitch.Y - a2DPitch.Y * b2DPitch.X; //normal.Z
    float anglePitch = std::acosf(dot) * sign;

    FVector result(0, anglePitch, angleYaw);
    return result;*/
}