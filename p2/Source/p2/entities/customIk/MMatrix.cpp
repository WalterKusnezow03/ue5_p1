// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include <cmath>
#include "p2/entities/customIk/MMatrix.h"

MMatrix::MMatrix()
{
    for (int i = 0; i < 16; i++){
        array[i] = 0.0f;
    }
    array[0] = 1.0f;
    array[5] = 1.0f;
    array[10] = 1.0f;
    array[15] = 1.0f;
}

MMatrix::~MMatrix()
{

}

/// @brief resets the rotation back to einheits matrix but not adjusting the translation
void MMatrix::resetRotation(){
    for (int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){
            int i = row * 4 + col; //obviously
            array[i] = 0.0f;
        }
    }
    //einheits matrix setzen
    array[0] = 1.0f;
    array[5] = 1.0f;
    array[10] = 1.0f;
    array[15] = 1.0f;
}




/// @brief copy constructor
/// @param other 
MMatrix::MMatrix(MMatrix &other){
    *this = other;
}


MMatrix& MMatrix::operator=(MMatrix &other){
    if(this == &other){
        return *this;
    }

    //copy data
    for (int i = 0; i < size; i++){
        array[i] = other.array[i];
    }

    return *this;
}

void MMatrix::setTranslation(FVector &other){
    //copy to correct positions in array 
    //(0,1,2,3)
    //(-,-,-,7)
    //(-,-,-,11)
    //(-,-,-,-) //last row stays emtpy
    array[3] = other.X;
    array[7] = other.Y;
    array[11] = other.Z;
}

void MMatrix::setTranslation(float x, float y, float z){
    array[3] = x;
    array[7] = y;
    array[11] = z;
}

FVector MMatrix::getTranslation(){
    FVector out;
    out.X = array[3];
    out.Y = array[7];
    out.Z = array[11];
    return out;
}

void MMatrix::operator+=(MMatrix &other){

}



/// @brief multiply with another matrix
/// @param other 
void MMatrix::operator*=(MMatrix &other){
    //old, berücksichtigt logik nicht, macht sachen kaputt

    normalizeRotation();
    other.normalizeRotation();

    /**
     * eine translation in den ursprung ist nicht notwendig 
     * bei den knochen weil die sowieso das sind.
     * 
     */
    
    MMatrix result; // Temporary matrix to store the result

    for (int row = 0; row < 4; row++) { // Iterate through the rows of 'this' matrix
        for (int col = 0; col < 4; col++) { // Iterate through the columns of 'other' matrix
            float sum = 0.0f;
            for (int inner = 0; inner < 4; inner++) { // Perform dot product
                sum += array[row * 4 + inner] * other.array[inner * 4 + col];
            }
            result.array[row * 4 + col] = sum; // Store the result in the temporary matrix
        }
    }

    // Copy the result back into this matrix
    for (int i = 0; i < 16; i++) {
        array[i] = result.array[i];
    }

    normalizeRotation();
}

/// @brief multiply and return result
/// @param other other
/// @return returns a new matrix
MMatrix MMatrix::operator*(MMatrix &other){
    normalizeRotation();
    other.normalizeRotation();
    MMatrix result; // Temporary matrix to store the result

    for (int row = 0; row < 4; row++) { // Iterate through the rows of 'this' matrix
        for (int col = 0; col < 4; col++) { // Iterate through the columns of 'other' matrix
            float sum = 0.0f;
            for (int inner = 0; inner < 4; inner++) { // Perform dot product
                sum += array[row * 4 + inner] * other.array[inner * 4 + col];
            }
            result.array[row * 4 + col] = sum; // Store the result in the temporary matrix
        }
    }
    result.normalizeRotation();
    return result;
}





/// @brief multiply with a vector 
/// @param other 
/// @return 
FVector MMatrix::operator*(FVector &other){
    FVector resultVec;

    float row[] = {other.X, other.Y, other.Z, 1.0f};
    float resultRow[] = {0.0f, 0.0f, 0.0f};
    
    for (int thisRow = 0; thisRow < 3; thisRow++) //jede row dieser matrix
    {
        float result = 0.0f;
        for (int c = 0; c < 4; c++){
            result += array[thisRow * 4 + c] * row[c];
        }
        resultRow[thisRow] = result;
    }
    

    resultVec.X = resultRow[0];
    resultVec.Y = resultRow[1];
    resultVec.Z = resultRow[2];
    return resultVec;
}


void MMatrix::operator+=(FVector &other){
    array[3] += other.X;
    array[7] += other.Y;
    array[11] += other.Z;
}
void MMatrix::operator-=(FVector &other){
    FVector copy = other * -1;
    *this += other;
}

/// @brief creates a string representation if the matrix with radian angles
/// @return FString by value representation of the matrix
FString MMatrix::asString(){
    FString output = "debug matrix: \n";
    for (int i = 0; i < 16; i++)
    {
        output += FString::Printf(TEXT(" %.2f"), (array[i]));
        if(i % 4 == 3){
            output += "\n";
        }
    }
    return output;
}





/// @brief util converts degrees to radian
/// @param deg deg input
/// @return 
float MMatrix::degToRadian(float deg){
    float angleInRadians = ((deg * M_PI) / 180.0); // Umwandlung in Bogenmaß
    return angleInRadians;
}

float MMatrix::radToDegree(float rad){
    float angleInDeg = ((rad * 180.0) / M_PI); // Umwandlung in Bogenmaß
    return angleInDeg;
}




/// @brief rotate along X in degree
/// @param a 
void MMatrix::roll(float deg){
    /*
    5  6
    9 10
    cos, -sin
    sin, cos
    */
    float torad = degToRadian(deg);
    rollRad(torad);
}

/// @brief rotate with y
/// @param a degree
void MMatrix::pitch(float deg){
    /*
    0  2
    8 10
    cos, sin
    -sin, cos
    */
    float torad = degToRadian(deg);
    pitchRad(torad);
}

void MMatrix::yaw(float deg){
    /*
    0  1
    4 5
    cos, -sin
    sin, cos
    */
    float torad = degToRadian(deg);
    yawRad(torad);
}


void MMatrix::rollRad(float a){
    /*
    5  6
    9 10
    cos, -sin
    sin, cos
    */
    float cos = std::cos(a);
    float sin = std::sin(a);

    array[5] = cos;
    array[9] = sin;
    array[6] = sin * -1;
    array[10] = cos;
}





/// @brief rotate with y
/// @param a
void MMatrix::pitchRad(float a){
    /*
    0  2
    8 10
    cos, sin
    -sin, cos
    */
    float cos = std::cos(a);
    float sin = std::sin(a);
    array[0] = cos;
    array[10] = cos;
    array[2] = sin;
    array[8] = sin * -1;
}

void MMatrix::yawRad(float a){
    /*
    0  1
    4 5
    cos, -sin
    sin, cos
    */
    float cos = std::cos(a);
    float sin = std::sin(a);

    array[0] = cos;
    array[5] = cos;
    array[4] = sin;
    array[1] = sin * -1;
}



/**
 * 
 * ADD
 * 
 */


/// @brief rotate along X in radian
/// @param a 
void MMatrix::rollRadAdd(float a){
    /*
    5  6
    9 10
    cos, -sin
    sin, cos
    */
    // Create a rotation matrix for rotation, then concatenate to not brick anything
    MMatrix rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[5] = cos;
    rotMatrix.array[9] = sin;
    rotMatrix.array[6] = sin * -1;
    rotMatrix.array[10] = cos;

    
    rotate(rotMatrix);
   
}

/// @brief rotate with y
/// @param a radian
void MMatrix::pitchRadAdd(float a){
    /*
    0  2
    8 10
    cos, sin
    -sin, cos
    */
    MMatrix rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);
    rotMatrix.array[0] = cos;
    rotMatrix.array[2] = sin;
    rotMatrix.array[8] = sin * -1;
    rotMatrix.array[10] = cos;
    
    rotate(rotMatrix);
    
}

void MMatrix::yawRadAdd(float a){
    /*
    0  1
    4 5
    cos, -sin
    sin, cos
    */
    MMatrix rotMatrix;
    float cos = std::cos(a);
    float sin = std::sin(a);

    rotMatrix.array[0] = cos;
    rotMatrix.array[1] = sin * -1;
    rotMatrix.array[4] = sin;
    rotMatrix.array[5] = cos;

    rotate(rotMatrix);
}

/// @brief factory method to create a rotation around x, y and z
/// @param x x roll radian rotation
/// @param y y pitch radian rotation
/// @param z z yaw radian rotation
/// @return MMatrix which will rotate another matrix around the desired rotation when multiplying
MMatrix MMatrix::createRotatorFromRad(float x, float y, float z){

    MMatrix result;

    result.rollRadAdd(x);
    result.pitchRadAdd(y);
    result.yawRadAdd(z);

   

    return result;
}


/// @brief factory method to create a rotation around x, y and z
/// @param x x roll degree rotation
/// @param y y pitch degree rotation
/// @param z z yaw degree rotation
/// @return MMatrix which will rotate another matrix around the desired rotation when multiplying
MMatrix MMatrix::createRotatorFromDeg(float x, float y, float z){
    x = MMatrix::degToRadian(x);
    y = MMatrix::degToRadian(y);
    z = MMatrix::degToRadian(z);
    return createRotatorFromRad(x, y, z);
}







/// @brief multiply with another matrix ROTATION ONLY
/// @param other 
void MMatrix::rotate(MMatrix &other){
    
    //Rges = R2 * R1 wenn Rges angewand werden soll und r1 als erstes angewandt
    
    MMatrix result; // Temporary matrix to store the result

    int limit = 3;

    for (int row = 0; row < limit; row++) { // Iterate through the rows of 'this' matrix
        for (int col = 0; col < limit; col++) { // Iterate through the columns of 'other' matrix
            
            float sum = 0.0f;
            for (int inner = 0; inner < limit; inner++) { // Perform dot product
                //sum += array[row * 4 + inner] * other.array[inner * 4 + col];
                sum += other.array[row * 4 + inner] * array[inner * 4 + col];
            }
            result.array[row * 4 + col] = sum; // Store the result in the temporary matrix
        }
    }

    // Copy the result back into this matrix

    for (int row = 0; row < 3; row++){
        for (int col = 0; col < 3; col++){

            int i = row * 4 + col;
            array[i] = result.array[i];
        }
    }
    normalizeRotation();
}










void MMatrix::normalizeRotation(){

    for (int row = 0; row < 3; row++) { // Iterate through the rows of 'this' matrix
        for (int col = 0; col < 3; col++) { // Iterate through the columns of 'other' matrix
            int i = row * 4 + col;
            float angleCopy = normalizeAngle(array[i]);
            array[i] = angleCopy;
        }
    }        
}


//damit keine werte über 360 grad entstehen

/// @brief normalizes the angle so none of it acts as scaling
/// @param angle angle in radian
/// @return 
float MMatrix::normalizeAngle(float angle) {
    // Normalisiere den Winkel auf den Bereich [0, 2π)
    bool flip = false;
    while (angle < 0){
        //angle += 2 * M_PI; // Sicherstellen, dass der Winkel positiv ist
        angle *= -1;
        flip = true;
    }

    float pi2 = 2 * M_PI;
    while (angle >= pi2){
        angle -= pi2; // Sicherstellen, dass er im Bereich liegt
    }

    if(flip){
        angle *= -1; //zurück flippen 
    }

    return angle;
}




void MMatrix::invert(){
    invertTranslation();
    invertRotation();
    //invertScale();
}


// -tx, -ty , -tz
void MMatrix::invertTranslation(){
    FVector translation = getTranslation();
    translation *= -1;
    setTranslation(translation);
}

//3x3 block transponieren für invertieren der rotation (da im orthogonalen koordinaten system)
void MMatrix::invertRotation(){
    //3x3 block transponieren
    swapIndices(4, 1);
    swapIndices(8, 2);
    swapIndices(9, 6);
}
/// @brief requires both indeces to be in bounds!
/// @param a 
/// @param b 
void MMatrix::swapIndices(int a, int b){
    if(a > 0 && b > 0 && a < 16 && b < 16){
        float copy = array[a];
        array[a] = array[a];
        array[b] = copy;
    }
}

void MMatrix::invertScale(){
    //diagonale 3 skalieren
    float a = array[0];
    float b = array[5];
    float c = array[10];

    if(a != 0)
        array[0] = 1.0f / a;

    if(b != 0)
        array[5] = 1.0f / b;

    if(c != 0)
        array[10] = 1.0f / c;
}
