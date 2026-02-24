// Fill out your copyright notice in the Description page of Project Settings.

#include "Matrix3x3.h"

#include "CoreMinimal.h"
#include <cmath>
#include "DebugPlugin/DebugHelper.h"
#include "Math/Rotator.h"


Matrix3x3::Matrix3x3()
{
    makeIdentity();
}

Matrix3x3::~Matrix3x3()
{

}



void Matrix3x3::makeIdentity(){
    makeZero();
    array[0] = 1.0f;
    array[4] = 1.0f;
    array[8] = 1.0f;
}

void Matrix3x3::makeZero(){
    for (int i = 0; i < size; i++){
        array[i] = 0.0f;
    }
}


Matrix3x3::Matrix3x3(FRotator &other){
    makeIdentity();
    setRotation(other);
}

/// @brief copy constructor
/// @param other 
Matrix3x3::Matrix3x3(const Matrix3x3 &other){
    makeIdentity();
    if (&other != this)
    {
        *this = other;
    }
}

//copy constructor
Matrix3x3& Matrix3x3::operator=(const Matrix3x3 &other){
    if(this == &other){
        return *this;
    }

    //copy data
    for (int i = 0; i < 9; i++){
        array[i] = other.array[i];
    }

    return *this;
}

void Matrix3x3::operator+=(Matrix3x3 &other){
    for (int i = 0; i < size; i++){
        array[i] += other.array[i];
    }
}

void Matrix3x3::operator-=(Matrix3x3 &other){
    for (int i = 0; i < size; i++){
        array[i] -= other.array[i];
    }
}

Matrix3x3 Matrix3x3::operator-(Matrix3x3 &other){
    Matrix3x3 result;
    for (int i = 0; i < size; i++){
        result.array[i] = array[i] - other.array[i];
    }
    return result;
}

/// @brief multiply with another matrix
/// @param other 
void Matrix3x3::operator*=(Matrix3x3 &other){
    Matrix3x3 r = *this * other;
    *this = r;
}

void Matrix3x3::operator*=(float scalar){
    for (int i = 0; i < size; i++){
        array[i] *= scalar;
    }
}


/// @brief multiply and return result
/// @param other other matrix to append like: This * other 
/// @return returns a new matrix
Matrix3x3 Matrix3x3::operator*(Matrix3x3 &other){
    
    Matrix3x3 result; // Temporary matrix to store the result

    for (int row = 0; row < 3; row++) { // Iterate through the rows of 'this' matrix
        for (int col = 0; col < 3; col++) { // Iterate through the columns of 'other' matrix
            float sum = 0.0f;
            for (int inner = 0; inner < 3; inner++) { // Perform dot product
                sum += array[row * 3 + inner] * other.array[inner * 3 + col];
            }
            result.array[row * 3 + col] = sum; // Store the result in the temporary matrix
        }
    }
    
    return result;
}





/// @brief multiply with a vector 
/// @param other 
/// @return 
FVector Matrix3x3::operator*(FVector &other){
    FVector resultVec;

    float row[] = {other.X, other.Y, other.Z};
    float resultRow[] = {0.0f, 0.0f, 0.0f};
    
    for (int thisRow = 0; thisRow < 3; thisRow++) //jede row dieser matrix
    {
        float result = 0.0f;
        for (int c = 0; c < 3; c++){
            result += array[thisRow * 3 + c] * row[c];
        }
        resultRow[thisRow] = result;
    }
    

    resultVec.X = resultRow[0];
    resultVec.Y = resultRow[1];
    resultVec.Z = resultRow[2];
    return resultVec;
}



/// @brief util converts degrees to radian
/// @param deg deg input
/// @return 
float Matrix3x3::degToRadian(float deg){
    float angleInRadians = ((deg * M_PI) / 180.0); // Umwandlung in Bogenmaß
    return angleInRadians;
}

float Matrix3x3::radToDegree(float rad){
    float angleInDeg = ((rad * 180.0) / M_PI); // Umwandlung in Bogenmaß
    return angleInDeg;
}


/**
 * 
 * 
 * SKEW
 * 
 * 
 */

/// @brief makes the skew matrix for omega
/// @param omega must be normalized!
void Matrix3x3::makeSkew(FVector &omega){
    //wenn man einen vektor mit der skew matrix multipliziert kommt
    //das kreuz produkt bei raus

    /*
    0  1  2
    3  4  5
    6  7  8
    */
    /*
              0   -wz    wy
    S(w) =   wz     0   -wx
            -wy   -wx     0
    */
    array[0] = 0.0f;
    array[1] = omega.Z * -1.0f;
    array[2] = omega.Y;
    array[3] = omega.Z;
    array[4] = 0.0f;
    array[5] = omega.X * -1.0f;
    array[6] = omega.Y * -1.0f;
    array[7] = omega.X;
    array[8] = 0.0f;

}

Matrix3x3 Matrix3x3::skew(FVector &omega){
    Matrix3x3 result;
    result.makeSkew(omega);
    return result;
}

Matrix3x3 Matrix3x3::makeExponentialMap_Skew(FVector &screw, float theta){
    /*
    s(w) ist skew von w
    
    e^s(w)*theta = 1_3x3 + s(w) * sin(theta) + s(w)^2 * (1-cos(theta))

    */
    
    Matrix3x3 identity;
    Matrix3x3 skewMat = skew(screw);
    Matrix3x3 skewMatQuadrat = skewMat * skewMat;
    float sine = std::sin(theta);
    float oneMinusCosine = 1.0f - std::cos(theta);

    skewMat *= sine;
    skewMatQuadrat *= oneMinusCosine;

    identity += skewMat;
    identity += skewMatQuadrat;
    return identity;
}

//convert to SE3 4x4
void Matrix3x3::convertPlueckerToSE3components(
    FVector &angularVelocity, //w0
    FVector &linearVelocity, //v0
    Matrix3x3 &outRotation, //delta rotation 
    FVector &outTranslation, //delta translation
    float deltatime //used for theta over time, angvelocity * deltatime
){
    /*
    extract transform in SE3:
        | R   tnew |
    T = | 0   1    |


    R = e^(Skew(angVelocity)dt)
    
    //hier ||w|| == 1 !! und theta_dt = w * dt
    tnew = (1_3x3 - eSkew) * (w x v)  +  (w dot v) * theta_dt * w
            ^^^^^rotation of screw^^^ | ^^^^ translation along screw^^^^^
    */


    //achtung: skew achse muss ja eigentlich normiert sein
    FVector angularVelocityNormalized = angularVelocity.GetSafeNormal();

    float thetaOverTime = angularVelocity.Size() * deltatime;
    Matrix3x3 expSkew = makeExponentialMap_Skew(angularVelocityNormalized, thetaOverTime);
    outRotation = expSkew;//.extractRotator();

    //t new extraction
    Matrix3x3 identity;
    identity -= expSkew;

    FVector wCrossV = FVector::CrossProduct(angularVelocity, linearVelocity);
    float wDotV = FVector::DotProduct(angularVelocity, linearVelocity);
    FVector wScaledDt = deltatime * angularVelocity;

    FVector rotationAroundScrew = identity * wCrossV;
    FVector translationAlongScrew = wDotV * wScaledDt;

    outTranslation = rotationAroundScrew + translationAlongScrew;
}


/**
 * convert T to Pluecker
 */
void Matrix3x3::convertSE3ToPluecker(
    Matrix3x3 &Rotation, //R
    FVector &translation, //p
    FVector &outAngularVelocity,
    FVector &outLinearVelocity,
    float wantedAngularVelocity
){
    /*
    l = (               
        r32 - r23,             
        r13 - r31, 
        r21 - r12
        )
    omega = l / (2*sin(theta)) //für s(omega)
    
    theta = sign(l dot p) * (acos((r11 + r22 + r33 - 1) / 2))

    h = (l · p)/(2 · θ · sin(θ))

    v = h * omega

    */

    //bestimmung xi(w,v)

    //falsch rum?
    FVector l(
        Rotation.getRowMajor(2,1) - Rotation.getRowMajor(1,2),
        Rotation.getRowMajor(0,2) - Rotation.getRowMajor(2,0),
        Rotation.getRowMajor(1,0) - Rotation.getRowMajor(0,1)
    );
    DebugHelper::showScreenMessage("l0", l);
    FVector l1(
        Rotation.get(2,1) - Rotation.get(1,2),
        Rotation.get(0,2) - Rotation.get(2,0),
        Rotation.get(1,0) - Rotation.get(0,1)
    );
    DebugHelper::showScreenMessage("l1", l1);



    FVector p = translation;
    float LPdot = FVector::DotProduct(l, p);
    float sign = LPdot >= 0.0f ? 1.0f : -1.0f;

    float inner = (
        Rotation.getRowMajor(0, 0) + 
        Rotation.getRowMajor(1, 1) + 
        Rotation.getRowMajor(2, 2) - 1
    ) / 2.0f;
    inner = FMath::Clamp(inner, -1.0f, 1.0f);
    float theta = sign * std::acosf(inner);

    float sinTheta = std::sin(theta);
    
    //void div by zero
    if(std::abs(sinTheta) < 0.000000000001f){
        outAngularVelocity = FVector(0.00001f,0.0f,0.0f);
        outLinearVelocity = translation;
        DebugHelper::showScreenMessage("sin theta not ok", (float)sinTheta);
        return;
    }
    if(std::abs(theta) < 0.000000000001f){
        outAngularVelocity = FVector::ZeroVector;
        outLinearVelocity = translation;
        DebugHelper::showScreenMessage("theta not ok", (float)theta);
        return;
    }
    
    FVector omega = l * (1.0f / (2.0f * sinTheta));

    float h = LPdot / (2.0f * theta * sinTheta);

    FVector w = omega; //* theta ? gute frage.
    FVector v = h * omega;

    DebugHelper::showScreenMessage("dir w", w.GetSafeNormal(), FColor::Green);

    outAngularVelocity = w; //gute frage.
    outLinearVelocity = v;

    //test
    //outAngularVelocity = w.GetSafeNormal() * wantedAngularVelocity;
    //outAngularVelocity = v.GetSafeNormal() * wantedAngularVelocity;
}











/**
 * copy
 */
std::vector<float> Matrix3x3::Copy(){
    std::vector<float> output = {
        array[0],
        array[1],
        array[2],
        array[3],
        array[4],
        array[5],
        array[6],
        array[7],
        array[8]
    };
    return output;
}

/**
 * 
 * ADD
 * 
 */


/// @brief rotate along X in radian
/// @param a 
void Matrix3x3::rollRadAdd(float a){
    if(a == 0.0f){
        return;
    }

    /*
    4  5
    7  8
    cos, -sin
    sin, cos
    */
    // Create a rotation matrix for rotation, then concatenate to not brick anything
    Matrix3x3 rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[4] = cos;
    rotMatrix.array[5] = sin;
    rotMatrix.array[7] = sin * -1;
    rotMatrix.array[8] = cos;

    rotate(rotMatrix);
}

/// @brief rotate with y
/// @param a radian
void Matrix3x3::pitchRadAdd(float a){
    if(a == 0.0f){
        return;
    }
    /*
    0  2
    6  8
    cos, sin
    -sin, cos
    */
    Matrix3x3 rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);
    rotMatrix.array[0] = cos;
    rotMatrix.array[2] = sin;
    rotMatrix.array[6] = sin * -1;
    rotMatrix.array[8] = cos;
    
    rotate(rotMatrix);
    
}

void Matrix3x3::yawRadAdd(float a){
    if(a == 0.0f){
        return;
    }

    /*
    0 1
    3 4
    cos, -sin
    sin, cos
    */
    Matrix3x3 rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[0] = cos;
    rotMatrix.array[1] = sin * -1;
    rotMatrix.array[3] = sin;
    rotMatrix.array[4] = cos;

    rotate(rotMatrix);
}





/// @brief multiply with another matrix ROTATION ONLY
/// @param other 
void Matrix3x3::rotate(Matrix3x3 &other){
    
    Matrix3x3 result; 
    for (int col = 0; col < 3; col++){
        for (int row = 0; row < 3; row++){

            float sum = 0.0f;
            for (int i = 0; i < 3; i++){
                float fromThis = get(i, row);
                float fromOther = other.get(col, i);
                sum += fromThis * fromOther;
            }
            result.set(col, row, sum);
        }
    
    }


    // Copy the result back into this matrix
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            set(col, row, result.get(col, row));
        }
    }




}


/// @brief resets the rotation and rotates in roll pitch yaw order
/// @param other rotation to rotate in
void Matrix3x3::setRotation(FRotator &other){
    if (FMath::IsNaN(other.Roll))
    {
        other.Roll = 0.0f;
    }
    if(FMath::IsNaN(other.Pitch)){
        other.Pitch = 0.0f;
    }
    if(FMath::IsNaN(other.Yaw)){
        other.Yaw = 0.0f;
    }
    makeIdentity();
    rollRadAdd(Matrix3x3::degToRadian(other.Roll));
    pitchRadAdd(Matrix3x3::degToRadian(other.Pitch));
    yawRadAdd(Matrix3x3::degToRadian(other.Yaw));
}

/// @brief will copy the rotation of the other matrix
/// @param other rotation to copy from matrix
void Matrix3x3::setRotation(Matrix3x3 &other){
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            set(i, j, other.get(i, j));
        }
    }
}

void Matrix3x3::setRotation(MMatrix &other){
    FRotator r = other.extractRotator();
    setRotation(r);
    return;

    //old
    std::vector<float> values = other.CopyRotation();
    if(values.size() == 9){
        for (int i = 0; i < 9; i++){
            array[i] = values[i];
        }
    }
}

/// @brief requires both indeces to be in bounds!
/// @param a 
/// @param b 
void Matrix3x3::swapIndices(int a, int b){
    if(a >= 0 && b >= 0 && a < size && b < size){
        float copy = array[a];
        array[a] = array[b];
        array[b] = copy;
    }
}








/// @brief extracts the rotator from this matrix
/// @return FRotator rotation of this matrix
FRotator Matrix3x3::extractRotator(){
    
    //extracting rotation from a matrix:
    /*

        r11 = a, 
        r12 = b,
        r13 = c

        r21 = e,
        r22 = f, 
        r23 = g

        r31 = i, 
        r32 = j,
        r33 = k


        M = {
            a b c d
            e f g h
            i j k l        
        }

        yaw = atan2(r21, r11)
        pitch = asin(-1 * r31)
        roll = atan2(r32, r33)

        yaw = atan2(b, a)
        pitch = asin(-1 * i)
        roll = atan2(j, k)


    */


    //kontext: get(column, row)
    float _yaw = std::atan2f(get(0, 1), get(0, 0));
    //float _pitch = -1 * std::asinf(get(0, 2));
    float _pitch = -1 * std::asinf(FMath::Clamp(get(0, 2), -1.0f, 1.0f));
    float _roll = std::atan2f(get(1, 2), get(2, 2));

    _yaw = MMatrix::radToDegree(_yaw);
    _pitch = MMatrix::radToDegree(_pitch);
    _roll = MMatrix::radToDegree(_roll);

    /*
    if(FMath::IsNaN(_yaw)){
        _yaw = 0.0f;
    }
    if(FMath::IsNaN(_pitch)){
        _pitch = 0.0f;
    }
    if(FMath::IsNaN(_roll)){
        _roll = 0.0f;
    }*/

    
    //keep rotation like this
    //FRotator Constructor expects FRotator(Yin, Zin, Rin)

    // fix, unreal is weird.
    // IS TESTED, THIS IS CORRECT, DONT PLAY WITH IT!!
    FRotator r(-1 * _pitch, _yaw, -1 * _roll);
    //FRotator r(-1 * _pitch, _yaw, -1 * _roll);
    //FRotator r(-1 * _pitch, _yaw, -1 * _roll);


    return r;

}



/**
 * 
 * new section for inverse
 * 
 */



void Matrix3x3::set(int column, int row, float value){
    bool lowerRange = column >= 0 && row >= 0;
    bool higherRange = column < 3 && row < 3;
    if(lowerRange && higherRange){
        int index = (row * 3) + column;
        array[index] = value;
    }
}

/// @brief 
/// @param columnX column to get X
/// @param rowY row to get Y
/// @return 
float Matrix3x3::get(int columnX, int rowY){
    
    bool lowerRange = columnX >= 0 && rowY >= 0;
    bool higherRange = columnX < 3 && rowY < 3;
    if(lowerRange && higherRange){
        int index = (rowY * 3) + columnX;
        return array[index];
    }
    return 0.0f;
}

float Matrix3x3::getRowMajor(int rowY, int columnX){
    return get(columnX, rowY);
}

/// @brief transposes this matrix as expected
void Matrix3x3::transpose(){
    /*
    0  1  2  
    3  4  5
    6  7  8
    */
    swapIndices(1, 3);
    swapIndices(2, 6);
    swapIndices(5, 7);
}







void Matrix3x3::setColumn(FVector &column, int i){
    i = std::min(2, i);
    i = std::max(0, i);

    set(i, 0, column.X);
    set(i, 1, column.X);
    set(i, 2, column.X);
}



/**
 * Inverse section
 */

/// @brief calculates the inverse matrix with jordan gaus algorythm
/// an identity matrix is returned if the inverse is not possible to make (det(A) = 0)
/// the inverse is calculated in O(n^2)
/// @return inverse matrix, or identity if an issue occured
Matrix3x3 Matrix3x3::jordanInverse(){
    Matrix3x3 identity; //operationen auf diese identity matrix auch anwenden,
    identity.makeIdentity();

    Matrix3x3 thisMatrix = *this; //kopie um nicht zu manipulieren
    // das ist am ende die inverse

    for (int x = 0; x < 3; x++){

        /*
        //durch pivot teilen dass 1 in pivot
        float devide = thisMatrix.get(x, x); //sollte das skallierungs element (sX, sY, sZ) = 0 sein
        //ist die matrix natürlich nicht invertierbar!!
        if(devide == 0.0f){
            DebugHelper::logMessage("debug identitydebug MATRIX NICHT INVERTIERBAR");
            MMatrix cleanMatrix;
            return cleanMatrix;
        }*/

        float devide = clampDivisionByZero(thisMatrix.get(x, x));
        thisMatrix.scaleRow(x, (1.0f / devide)); // d * 1 / d = 1, skallieren pivot.
        identity.scaleRow(x, (1.0f / devide));

        //darunter wandern, elemente eliminieren
        for (int y = x + 1; y < 3; y++){

            //gleichung essenziell:
            //a soll 0 sein

            //1 - - - 
            //a - - - 
            //b - - - 
            //c - - -
            // usw

            // a - x * 1 = 0 
            // a = x * 1
            //heisst dann ja arow -= 1row * x und x ist das spalten element darunter
            float lower = thisMatrix.get(x, y);
            thisMatrix.minusForRow(y, x, lower); //von untere, jetzt (1 * something sodass = 0, = something, ist gleich wert.)
            identity.minusForRow(y, x, lower);
        }
    }

    // Eliminate elements above the pivots (new part)
    for (int x = 2; x >= 0; x--) {
        for (int y = x - 1; y >= 0; y--) {
            float upper = thisMatrix.get(x, y);
            thisMatrix.minusForRow(y, x, upper); // Eliminate element in (y, x)
            identity.minusForRow(y, x, upper); // Do the same for identity matrix
        }
    }
    

    /*
    FString message = FString::Printf(TEXT("debug inverse:"));
	message += identity.asString();
    DebugHelper::logMessage(message);
    */

    return identity;
}

/// @brief a very dangerous fix to let the matrix be invertable. Does introduce numeric
/// inaccuracy
/// THIS VALUES ARE TESTED FOR THE BONECONTROLLER AND SEEMS TO WORK OK! DO NOT CHANGE!
/// @param other value to check
/// @return value to allow invertion of the matrix with jordan gauß verfahren
float Matrix3x3::clampDivisionByZero(float other){
    float min = 0.000000000001f; // 0.000001f
    if (std::abs(other) <= min)
    {
        return min;
    }
    return other;
}


/// @brief scales a row with a factor 
/// @param row row to scale
/// @param scale faktor
void Matrix3x3::scaleRow(int row, float scale){
    for (int i = 0; i < 3; i++){
        float val = get(i, row) * scale;
        set(i, row, val);
    }
}

/// @brief subtrahiere von row , other row, mal einem faktor
/// @param row row to apply subtraction
/// @param otherRow subtract this row
/// @param faktor faktor of otherRow 
void Matrix3x3::minusForRow(int row, int otherRow, float faktor) {

    for (int i = 0; i < 3; i++){
        float otherValueScaled = get(i, otherRow) * faktor;
        float thisValue = get(i, row) - otherValueScaled;
        set(i, row, thisValue);
    }
}



FString Matrix3x3::asString(){
    FString out;
    for (int i = 0; i < 3; i++)
    {
        FString s = FString::Printf(
            TEXT("[%.2f, %.2f, %.2f]"),
            get(i, 0),
            get(i, 1),
            get(i, 2)
        );
        out += s;
    }
    return out;
}



void Matrix3x3::testInverse(){
    Matrix3x3 a;
    a.makeIdentity();
    a.yawRadAdd(Matrix3x3::degToRadian(23));
    a.pitchRadAdd(Matrix3x3::degToRadian(54));
    a.rollRadAdd(Matrix3x3::degToRadian(-30));
    Matrix3x3 aInv = a.jordanInverse();

    Matrix3x3 result = a * aInv;
    FString s = result.asString();
    DebugHelper::logMessage("inverse test ", s);


    FVector aVec(4, 3, 2);
    FVector bVec = result * aVec;
    
    if(FVector::Dist(aVec, bVec) < 1.0f){
        DebugHelper::logMessage("inverse test valid", aVec, bVec);
    }

}