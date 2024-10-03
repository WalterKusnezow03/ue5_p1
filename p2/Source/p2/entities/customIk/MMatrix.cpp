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

}


/// @brief multiply and return result
/// @param other other
/// @return returns a new matrix
MMatrix MMatrix::operator*(MMatrix &other){
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


/// @brief rotate along X in degree
/// @param a 
void MMatrix::roll(float a){
    /*
    5  6
    9 10
    cos, -sin
    sin, cos
    */
    float torad = degToRadian(a);
    array[5] = std::cos(torad);
    array[9] = std::sin(torad);
    array[6] = array[9] * -1;
    array[10] = array[5];
}

/// @brief rotate with y
/// @param a degree
void MMatrix::pitch(float a){
    /*
    0  2
    8 10
    cos, sin
    -sin, cos
    */
    float torad = degToRadian(a);
    array[0] = std::cos(torad);
    array[10] = array[0];
    array[2] = std::sin(torad);
    array[8] = array[2] * -1;
}

void MMatrix::yaw(float a){
    /*
    0  1
    4 5
    cos, -sin
    sin, cos
    */
    float torad = degToRadian(a);
    array[0] = std::cos(torad);
    array[5] = array[0];
    array[4] = std::sin(torad);
    array[1] = array[4] * -1;
}




/// @brief rotate along X in degree
/// @param a 
void MMatrix::rollRad(float a){
    /*
    5  6
    9 10
    cos, -sin
    sin, cos
    */
    array[5] = std::cos(a);
    array[9] = std::sin(a);
    array[6] = array[9] * -1;
    array[10] = array[5];
}

/// @brief rotate with y
/// @param a degree
void MMatrix::pitchRad(float a){
    /*
    0  2
    8 10
    cos, sin
    -sin, cos
    */
    array[0] = std::cos(a);
    array[10] = array[0];
    array[2] = std::sin(a);
    array[8] = array[2] * -1;
}

void MMatrix::yawRad(float a){
    /*
    0  1
    4 5
    cos, -sin
    sin, cos
    */
    array[0] = std::cos(a);
    array[5] = array[0];
    array[4] = std::sin(a);
    array[1] = array[4] * -1;
}










/// @brief util converts degrees to radian
/// @param deg deg input
/// @return 
float MMatrix::degToRadian(float deg){
    float angleInRadians = deg * (M_PI / 180.0); // Umwandlung in Bogenmaß
    return angleInRadians;
}

float MMatrix::radToDegree(float rad){
    float angleInDeg = rad * (180.0 / M_PI); // Umwandlung in Bogenmaß
    return angleInDeg;
}