// Fill out your copyright notice in the Description page of Project Settings.


#include "FVectorShape.h"
#include "p2/meshgen/MeshData.h"
#include "p2/meshgen/generation/bezierCurve.h"
#include "GrahamScan.h"
#include "p2/util/TVector.h"

FVectorShape::FVectorShape()
{
}

FVectorShape::~FVectorShape()
{
}

FVectorShape::FVectorShape(const FVectorShape &other){
    if(&other != this){
        *this = other;
    }
}

FVectorShape::FVectorShape(const FVectorShape &other, MMatrix &applyOffset){
    if(&other != this){
        *this = other;
    }
    moveVerteciesWith(applyOffset);
}

FVectorShape &FVectorShape::operator=(const FVectorShape &other){
    if(&other == this){
        return *this;
    }
    vec = other.vec;
    return *this;
}

/// @brief moves the vertecies with a desired matrix
/// @param mat some matrix
void FVectorShape::moveVerteciesWith(MMatrix &mat){
    for (int i = 0; i < vec.size(); i++){
        FVector &current = vec[i];
        vec[i] = mat * current;
    }
}



void FVectorShape::push_back(FVector other){
    vec.push_back(other);
}

void FVectorShape::push_back(FVectorShape &other){
    for (int i = 0; i < other.vec.size(); i++){
        vec.push_back(other.vec[i]);
    }
}

void FVectorShape::push_back(std::vector<FVector> &other){
    for (int i = 0; i < other.size(); i++){
        vec.push_back(other[i]);
    }
}


/// @brief joins the previous vertecies added without bricking the illumnation / shading
/// @param other mesh data to append smoothley to
void FVectorShape::joinMeshData(MeshData &other){
    other.appendVertecies(vec);
}

/// @brief creates a double sided mesh from own shape and center of mass
/// @return shape Mesh data, double sided, with calculated normals done!
MeshData FVectorShape::createDoubleSidedMesh(){
    MeshData mesh;
    
    //center of mass
    FVector center = calculateCenter();

    // connect triangles to center
    for (int i = 0; i < vec.size(); i++){
        //create double sided triangle
        int nextIndex = (i + 1) % vec.size();
        FVector &current = vec[i];
        FVector &next = vec[nextIndex];
        mesh.appendDoublesided(center, current, next);
    }

    mesh.calculateNormals();
    return mesh;
}


/// @brief closes mesh at center: clockwise means up, counter clockwise is down dir (bool flag)
/// @param clockWise clockwise means up, counter clockwise is down dir (bool flag)
/// @return meshdata, with calculated normals done!
MeshData FVectorShape::closeMeshAtCenter(bool clockWise){
    MeshData mesh;
    
    //center of mass
    FVector center = calculateCenter();
    mesh.closeMeshAtCenter(center, vec, clockWise);
    mesh.calculateNormals();
    return mesh;

}


/// @brief calculates the center of all vertecies
/// @return center
FVector FVectorShape::calculateCenter(){
    FVector center;
    for (int i = 0; i < vec.size(); i++){
        center += vec[i];
    }
    center /= vec.size();
    return center;
}

/// @brief creates vertecies in mmatrix represantation
/// @param output 
void FVectorShape::copyVertecies(std::vector<MMatrix> &output){
    for (int i = 0; i < vec.size(); i++){
        MMatrix mat;
        mat.setTranslation(vec[i]);
        output.push_back(mat);
    }
}




/**
 * new rand function for rocks
 */
void FVectorShape::randomizeVertecies(int maxdistance){

    FVector center = calculateCenter();
    for (int i = 0; i < vec.size(); i++)
    {
        FVector &current = vec[i];
        FVector dir = center - current;
        dir = dir.GetSafeNormal();

        current += dir * FVectorUtil::randomNumber(-maxdistance, maxdistance);
    }
}




/// @brief will smooth the shape with a detail of avg distance * 10
void FVectorShape::smoothWithBezier(){
    //distance on average
    if(vec.size() == 0){
        return;
    }
    float distance = 0.0f;
    for (int i = 0; i < vec.size(); i++){
        int nextIndex = (i + 1) % vec.size();
        distance += FVector::Dist(vec[i], vec[nextIndex]);
    }
    distance /= vec.size();

    //say 4 for example
    smoothWithBezier(distance / 10);
}

void FVectorShape::smoothWithBezier(int detailStep){
    if(vec.size() == 0){
        return;
    }

    std::vector<FVector> output;
    bezierCurve curve;
    curve.calculatecurve(
        vec,
        output,
        detailStep // einheits value
    );

    vec = output;
}



int FVectorShape::vertexCount(){
    return vec.size();
}



void FVectorShape::keepVertexCountFromFront(int countLimit){
    if(vec.size() == 0){
        return;
    }
    if(countLimit >= vec.size()){
        return;
    }

    vec.erase(vec.begin() + countLimit, vec.end());
}




/// @brief moves all vertecies so the pivot is the center
void FVectorShape::makeCenterPivot(){
    FVector centerToZero = -1 * calculateCenter();
    if(centerToZero.Size() == 0.0f){
        return;
    }

    MMatrix moveToCenter;
    moveToCenter.setTranslation(centerToZero);
    moveVerteciesWith(moveToCenter);
}




/**
 * 
 * 
 * --- helper functions for terrain ---
 * 
*/

std::vector<FVector> FVectorShape::vectorCopy(){
    return vec;
}

void FVectorShape::createRandomNewSmoothedShape(int sizeXYMax, int smoothStep){
    vec.clear();
    int halfsize = sizeXYMax / 2;

    int detail = 8;
    for (int i = 0; i < detail; i++)
    {
        MMatrix rot;
        int part = 360 / detail;
        rot.yawRadAdd(MMatrix::degToRadian(part * i));
        std::vector<FVector> newQuad = MeshData::create2DQuadVertecies(halfsize, halfsize);
        for (int j = 0; j < newQuad.size(); j++){
            FVector &current = newQuad[j];
            current = rot * current;
            vec.push_back(current);
        }
    }
    GrahamScan grahamscan;
    grahamscan.ComputeConvexHull(vec);

    randomizeVertecies(sizeXYMax / 10);

    smoothWithBezier(smoothStep);
}


void FVectorShape::createRandomNewSmoothedShapeClamped(int sizeXYMax, int smoothStep){
    createRandomNewSmoothedShape(sizeXYMax, smoothStep);

    /*
    int limit = sizeXYMax - 1;
    for (int i = 0; i < vec.size(); i++)
    {
        FVector &current = vec[i];
        clampVector(current, 0, limit);
    }*/
}

void FVectorShape::clampVector(FVector &pos, int minValue, int maxValue){
    if(pos.X < minValue){
        pos.X = minValue;
    }
    if(pos.Y < minValue){
        pos.Y = minValue;
    }

    if(pos.X > maxValue){
        pos.X = maxValue;
    }
    if(pos.Y > maxValue){
        pos.Y = maxValue;
    }
}

void FVectorShape::floorAllCoordinateValues(){
    for (int i = 0; i < vec.size(); i++){
        FVector &current = vec[i];
        current.X = std::floor(current.X);
        current.Y = std::floor(current.Y);
        current.Z = std::floor(current.Z);
    }
}



void FVectorShape::sortVerteciesOnXAxis(){
    try {
        std::sort(vec.begin(), vec.end(), [](const FVector &a, const FVector &b) {
            if(a.X == b.X){
                return a.Y < b.Y;
            }
            return a.X < b.X;
        });
    }
    catch (const std::exception &e) {
        DebugHelper::logMessage("error in sorting fvectorshape on xaxis");
    }
}

