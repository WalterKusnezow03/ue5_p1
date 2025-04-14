#include "MMatrix3x3.h"


MMatrix3x3::MMatrix3x3(){
    makeIdentity();
}

MMatrix3x3::~MMatrix3x3(){

}

MMatrix3x3::MMatrix3x3(const MMatrix3x3 &other){
    if(&other != this){
        *this = other;
    }
}

MMatrix3x3::MMatrix3x3(FRotator &other){
    makeIdentity();

    float roll = MMatrix3x3::degToRadian(other.Roll);
    float pitch = MMatrix3x3::degToRadian(other.Pitch);
    float yaw = MMatrix3x3::degToRadian(other.Yaw);

    rollRadAdd(roll);
    pitchRadAdd(pitch);
    yawRadAdd(yaw);

}


MMatrix3x3 &MMatrix3x3::operator=(const MMatrix3x3 &other){
    if(&other != this){
        for (int i = 0; i < 9; i++){
            array[i] = other.array[i];
        }
    }
    return *this;
}

/// @brief multiply and return result
/// @param other other matrix to append like: This * other 
/// @return returns a new matrix
MMatrix3x3 MMatrix3x3::operator*(const MMatrix3x3 &other){
    
    MMatrix3x3 result; // Temporary matrix to store the result

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

MMatrix3x3 MMatrix3x3::operator+(const MMatrix3x3 &other){
    //CIJ = AIJ + BIJ
    MMatrix3x3 result;
    for (int i = 0; i < 9; i++){
        result.array[i] = array[i] + other.array[i];
    }
    return result;
}

FVector MMatrix3x3::operator*(FVector &other){
    FVector resultVec;

    float row[] = {other.X, other.Y, other.Z};
    float resultCol[] = {0.0f, 0.0f, 0.0f};
    
    for (int thisRow = 0; thisRow < 3; thisRow++) //jede row dieser matrix
    {
        float result = 0.0f;
        for (int c = 0; c < 3; c++){
            result += array[thisRow * 3 + c] * row[c];
        }
        resultCol[thisRow] = result;
    }
    

    resultVec.X = resultCol[0];
    resultVec.Y = resultCol[1];
    resultVec.Z = resultCol[2];
    return resultVec;
}



void MMatrix3x3::operator*=(MMatrix3x3 &other){
    MMatrix3x3 result; // Temporary matrix to store the result

    for (int row = 0; row < 3; row++) { // Iterate through the rows of 'this' matrix
        for (int col = 0; col < 3; col++) { // Iterate through the columns of 'other' matrix
            float sum = 0.0f;
            for (int inner = 0; inner < 3; inner++) { // Perform dot product
                sum += array[row * 3 + inner] * other.array[inner * 3 + col];
            }
            result.array[row * 3 + col] = sum; // Store the result in the temporary matrix
        }
    }

    *this = result;
}

/// @brief cij = aij + bij
/// @param other 
void MMatrix3x3::operator+=(MMatrix3x3 &other){
    for (int i = 0; i < 9; i++){
        array[i] += other.array[i];
    }
}

void MMatrix3x3::operator*=(float skalar){
    for (int i = 0; i < 9; i++){
        array[i] *= skalar;
    }
}




void MMatrix3x3::makeIdentity(){
    /*
    0 1 2
    3 4 5 
    6 7 8
    */
    for (int i = 0; i < 9; i++){
        array[i] = 0.0f;
    }
    array[0] = 1.0f;
    array[4] = 1.0f;
    array[8] = 1.0f;
}







/// @brief creates the skew matrix for a vector (kreuz produkt matrix)
/// @param other vector
/// @return skew matrix / kreuz produkt matrix
MMatrix3x3 MMatrix3x3::skewMatrix(FVector &other){
    MMatrix3x3 skew;
    /*
    
          0  -az  ay
    ax = az    0 -ax
        -ay   ax   0
    
    */
    skew.array[0] = 0.0f;
    skew.array[1] = -other.Z;
    skew.array[2] = other.Y;

    skew.array[3] = other.Z;
    skew.array[4] = 0.0f;
    skew.array[5] = -other.X;

    skew.array[6] = -other.Y;
    skew.array[7] = other.X;
    skew.array[8] = 0.0f;




    return skew;
}








/**
 * 
 * Rotation
 * 
 */

/// @brief util converts degrees to radian
/// @param deg deg input
/// @return 
float MMatrix3x3::degToRadian(float deg){
    float angleInRadians = ((deg * M_PI) / 180.0); // Umwandlung in Bogenmaß
    return angleInRadians;
}

float MMatrix3x3::radToDegree(float rad){
    float angleInDeg = ((rad * 180.0) / M_PI); // Umwandlung in Bogenmaß
    return angleInDeg;
}


/// @brief rotate along X in radian
/// @param a 
void MMatrix3x3::rollRadAdd(float a){
    if(a == 0.0f){
        return;
    }
    /*
    0 1 2
    3 4 5
    6 7 8
    */

    /*
    4  5
    7  8
    cos, -sin
    sin, cos
    */
    // Create a rotation matrix for rotation, then concatenate to not brick anything
    MMatrix3x3 rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[4] = cos;
    rotMatrix.array[7] = sin;
    rotMatrix.array[5] = sin * -1;
    rotMatrix.array[8] = cos;

    *this *= rotMatrix;
}

/// @brief rotate with y
/// @param a radian
void MMatrix3x3::pitchRadAdd(float a){
    if(a == 0.0f){
        return;
    }

    /*
    0 1 2
    3 4 5
    6 7 8
    */

    /*
    0  2
    6  8
    cos, sin
    -sin, cos
    */
    MMatrix3x3 rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);
    rotMatrix.array[0] = cos;
    rotMatrix.array[2] = sin;
    rotMatrix.array[6] = sin * -1.0f;
    rotMatrix.array[8] = cos;
    
    *this *= rotMatrix;
    
}

void MMatrix3x3::yawRadAdd(float a){
    if(a == 0.0f){
        return;
    }

    /*
    0 1 2
    3 4 5
    6 7 8
    */

    /*
    0  1
    3  4
    cos, -sin
    sin, cos
    */
    MMatrix3x3 rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[0] = cos;
    rotMatrix.array[1] = sin * -1;
    rotMatrix.array[3] = sin;
    rotMatrix.array[4] = cos;

    *this *= rotMatrix;
}









/**
 * 
 * rotator extraction
 * 
 */



/// @brief creates the rotator from a given vector in pitch and yaw rotation! caution: both relative axis of measurement
/// are the x axis for yaw and pitch!! (1,0) (1,0)!!
/// @param other vector (direction) - will be normalized internally.
/// @return rotator matrix
MMatrix3x3 MMatrix3x3::createRotatorFrom(FVector &other){
    FVector2D Xaxis(1.0f, 0); //default axis, x nach vorne schauend
    return createRotatorFrom(other, Xaxis, Xaxis); 
}


/// @brief 
/// @param other 
/// @param XAxis axis to measure yaw agains
/// @param ZAxis axis to measure pitch against
/// @return 
MMatrix3x3 MMatrix3x3::createRotatorFrom(FVector &other, FVector2D XAxis, FVector2D ZAxis){
    FVector normalized = other.GetSafeNormal();

    //yaw angle
    FVector2D xydir(normalized.X, normalized.Y);
    float yawRad = signedAngleRadBetween(XAxis, xydir); //signiert voll umfänglich notwendig anders als pitch

    //idee: yaw angle raus nehmen damit der pitch korrekt projeziert ist! (das ist richtig so!)
    MMatrix removeYaw;
    removeYaw.yawRadAdd(-yawRad);
    normalized = removeYaw * normalized;
    normalized = normalized.GetSafeNormal();

    //pitch angle
    FVector2D xzdir(normalized.X, normalized.Z);
    float pitchRad = signedAngleRadBetween(ZAxis, xzdir) * -1.0f; //test (negieren. Aus. Gründen.) 

    


    MMatrix3x3 rotationMat;
    rotationMat.yawRadAdd(yawRad);
    rotationMat.pitchRadAdd(pitchRad);

    return rotationMat;
}



float MMatrix3x3::unsignedAngleRadBetween(FVector2D &a, FVector2D &b){
    a = a.GetSafeNormal();
    b = b.GetSafeNormal();
    float cosine = a.X * b.X + a.Y * b.Y;
    //cosine = std::clamp(cosine, -1.0f, 1.0f);
    float angle = std::acosf(cosine);
    return angle;
}

float MMatrix3x3::signedAngleRadBetween(FVector2D &a, FVector2D &b){
    float angle = unsignedAngleRadBetween(a,b) * signForAngle(a, b);
    return angle;
}

/// @brief returns the sign if a and b would be on clockwise order or not, -1 (clockwise angle): 1
/// @param a 
/// @param b 
/// @return 
float MMatrix3x3::signForAngle(FVector2D &a, FVector2D &b){
    /*
    --------ax   bx
    ay   by
    az   bz
    ax   bx
    ay   by
    --------az   bz

    */
    float normalComponentZ = a.X * b.Y - a.Y * b.X;
    if(normalComponentZ < 0.0f){ //clockwise, normale zeigt nach unten
        return -1.0f;
    }
    return 1.0f;
}




/**
 * rotaion extraction 
 */
void MMatrix3x3::operator/=(int skalar){
    if(skalar == 0){
        return;
    }
    float asFloat = skalar;
    for (int i = 0; i < 9; i++)
    {
        array[i] /= asFloat;
    }
}


MMatrix3x3 MMatrix3x3::expMatrix(int detail){
    
    //exp(A) = I + (A^1 / 1!) + (A*A) / 2! + ...
    
    
    std::vector<int> fakultaetValues;
    fakultaet(detail, fakultaetValues);

    MMatrix3x3 IdentityAndOutput;
    MMatrix3x3 self = *this;
    MMatrix3x3 prevQuadStep = IdentityAndOutput; // A^x, start with one, nothing happens
    for (int i = 0; i < fakultaetValues.size(); i++){
        MMatrix3x3 denominator = prevQuadStep * self;
        prevQuadStep = denominator; //copy for next! ohne bruch!

        int counter = fakultaetValues[i];

        denominator /= counter;
        IdentityAndOutput += denominator;
    }

    return IdentityAndOutput;
}

void MMatrix3x3::fakultaet(int value, std::vector<int> &output){

    //3! = 3 * 2 * 1 oder 3! = 3 * 2!

    //Calculate factorial values (1!, 2!, 3!, ...)
    int fact = 1;
    output.push_back(fact);  // 0! = 1
    
    for (int i = 1; i <= value; ++i) {
        fact *= i;
        output.push_back(fact);
    }
}







/**
 * extract rotator
 */

/// @brief extracts the rotator from this matrix
/// @return FRotator rotation of this matrix
FRotator MMatrix3x3::extractRotator(){
    
    //rotation wird mithilfe von unreal extrahiert 
    //weil ich es selber nicht weiss
    /*
    //so korrekt
    FMatrix MyMatrix(
        FPlane(get(0, 0), get(0, 1), get(0, 2), get(0, 3)), // erste Spalte
        FPlane(get(1, 0), get(1, 1), get(1, 2), get(1, 3)), // zweite Spalte
        FPlane(get(2, 0), get(2, 1), get(2, 2), get(2, 3)), // dritte Spalte
        FPlane(get(3, 0), get(3, 1), get(3, 2), get(3, 3))  // vierte Spalte
    );
    return MyMatrix.Rotator();
    */

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




void MMatrix3x3::set(int column, int row, float value){
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
float MMatrix3x3::get(int columnX, int rowY){
    
    bool lowerRange = columnX >= 0 && rowY >= 0;
    bool higherRange = columnX < 3 && rowY < 3;
    if(lowerRange && higherRange){
        int index = (rowY * 3) + columnX;
        return array[index];
    }
    return 0.0f;
}