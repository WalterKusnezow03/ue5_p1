#include "FTriangleClipResult.h"


bool FTriangleClipResult::BoundsTest(
    const FVector2D &halfBound, 
    const FVector &vertex0,
    const FVector &vertex1,
    const FVector &vertex2
){
    halfBoundsCopied = halfBound;
    fullBounds = halfBound * 2.0;
    planeBuffer = GenerateBoundingPlanes();
    v0InBound = IsInBound(halfBound, vertex0);
    v1InBound = IsInBound(halfBound, vertex1);
    v2InBound = IsInBound(halfBound, vertex2);

    v0 = vertex0;
    v1 = vertex1;
    v2 = vertex2;

    return v0InBound || v1InBound || v2InBound;
}

bool FTriangleClipResult::IsInBound(
    const FVector2D &halfBound, 
    const FVector &vertex
){
    return 
    vertex.X > -halfBound.X &&
    vertex.Y > -halfBound.Y &&
    vertex.X < halfBound.X &&
    vertex.Y < halfBound.Y;
}


void FTriangleClipResult::Clip(){
    if(CountTrue() == 3){
        //all in bound
        generatedTriangleShapedVertexBuffer.Add(v0);
        generatedTriangleShapedVertexBuffer.Add(v1);
        generatedTriangleShapedVertexBuffer.Add(v2);
        return;
    }

    if(OneVertexInBound()){
        CaseOneVertexInBound();
        return;
    }
    if(TwoVerteciesInBound()){
        CaseTwoInBound();
        return;
    }

    
}




TArray<FPlane> FTriangleClipResult::GenerateBoundingPlanes(){
    TArray<FPlane> planes;

    FVector PlanePoint = FVector(-halfBoundsCopied.X,0,0);
    FVector PlaneNormal = FVector(-1,0,0);
    planes.Add(FPlane(PlanePoint, PlaneNormal));


    PlanePoint = FVector(halfBoundsCopied.X,0,0);
    PlaneNormal = FVector(1,0,0);
    planes.Add(FPlane(PlanePoint, PlaneNormal));


    PlanePoint = FVector(0,-halfBoundsCopied.Y,0);
    PlaneNormal = FVector(0,-1,0);
    planes.Add(FPlane(PlanePoint, PlaneNormal));


    PlanePoint = FVector(0,halfBoundsCopied.Y,0);
    PlaneNormal = FVector(0,1,0);
    planes.Add(FPlane(PlanePoint, PlaneNormal));

    return planes;
}

void FTriangleClipResult::CaseOneVertexInBound(){
    /*
    1--->2
    |  -
    0<-
    */

    //gegen alle vertikalen ebenen schneiden
    //dreieck dem entsprechend verkleinern
    //winding order beachten
    if(v0InBound){
        ClipOne(v0, v1, v2);
        return;
    }
    if(v1InBound){
        ClipOne(v1, v2, v0);
        return;
    }
    if(v2InBound){
        ClipOne(v2, v0, v1);
        return;
    }
}

void FTriangleClipResult::ClipOne(const FVector &inBound, const FVector &v1In, const FVector &v2In){
    FVector v1Cut = v1In;
    for (int i = 0; i < planeBuffer.Num(); i++){
        FVector Intersection;
        if(FMath::SegmentPlaneIntersection(
            inBound, //StartPoint,
            v1In, //End
            planeBuffer[i], //Plane,
            Intersection
        )){
            v1Cut = Intersection;
            break;
        }
    }

    FVector v2Cut = v2In;
    for (int i = 0; i < planeBuffer.Num(); i++){
        FVector Intersection;
        if(FMath::SegmentPlaneIntersection(
            inBound, //StartPoint,
            v2In, //End
            planeBuffer[i], //Plane,
            Intersection
        )){
            v2Cut = Intersection;
            break;
        }
    }

    /*
    1--->2
    |  -
    0<-
    */
    generatedTriangleShapedVertexBuffer.Add(inBound);
    generatedTriangleShapedVertexBuffer.Add(v1Cut);
    generatedTriangleShapedVertexBuffer.Add(v2Cut);
}

void FTriangleClipResult::CaseTwoInBound(){
    if(v0InBound && v1InBound){
        //Correct!
        ClipTwo(v1, v0, v2);

        //wrong???
        //ClipTwo(v0, v1, v2);
        return;
    }

    if(v0InBound && v2InBound){
        //correct!
        ClipTwo(v0, v2, v1);
        
        //WRONG
        //ClipTwo(v2, v0, v1);
        return;
    }

    if(v1InBound && v2InBound){
        //correct!
        ClipTwo(v2, v1, v0);

        //WRONG!
        //ClipTwo(v1, v2, v0);
        return;
    }

}







void FTriangleClipResult::ClipTwo(const FVector &inBoundV0, const FVector &inBoundV1, const FVector &v2In){
    FVector v0v2Cut = inBoundV0;
    for (int i = 0; i < planeBuffer.Num(); i++){
        FVector Intersection;
        if(FMath::SegmentPlaneIntersection(
            inBoundV0, //StartPoint,
            v2In, //End
            planeBuffer[i], //Plane,
            Intersection
        )){
            v0v2Cut = Intersection;
            break;
        }
    }

    FVector v1v2Cut = inBoundV1;
    for (int i = 0; i < planeBuffer.Num(); i++){
        FVector Intersection;
        if(FMath::SegmentPlaneIntersection(
            inBoundV1, //StartPoint,
            v2In, //End
            planeBuffer[i], //Plane,
            Intersection
        )){
            v1v2Cut = Intersection;
            break;
        }
    }

    //quad is generated from 2 cut points

    //richtig drehen
    /*FVector normal = FVector::CrossProduct(v0v2Cut - inBoundV0, inBoundV1 - inBoundV0);
    if(FVector::DotProduct(normal, FVector(0,0,1)) < 0){
        //std::swap(v0v2Cut, inBoundV1);
        FVector copy = v0v2Cut;
        v0v2Cut = inBoundV1;
        inBoundV1 = copy;
    }*/

    //triangle A
    generatedTriangleShapedVertexBuffer.Add(inBoundV0);
    generatedTriangleShapedVertexBuffer.Add(v0v2Cut);
    generatedTriangleShapedVertexBuffer.Add(inBoundV1);

    //triangle B
    generatedTriangleShapedVertexBuffer.Add(inBoundV1);
    generatedTriangleShapedVertexBuffer.Add(v0v2Cut);
    generatedTriangleShapedVertexBuffer.Add(v1v2Cut);
}














bool FTriangleClipResult::OneVertexInBound(){
    return CountTrue() == 1;
}

bool FTriangleClipResult::TwoVerteciesInBound(){
    return CountTrue() == 2;
}

int FTriangleClipResult::CountTrue(){
    TArray<bool> array = {v0InBound, v1InBound, v2InBound};
    int count = 0;
    for (int i = 0; i < array.Num(); i++){
        if(array[i]){
            count++;
        }
    }
    return count;
}

bool FTriangleClipResult::AnyInBound(){
    return CountTrue() > 0;
}

void FTriangleClipResult::AppendTo(MeshData &data){
    MakeUVBuffer();

    for (int i = 2; i < generatedTriangleShapedVertexBuffer.Num(); i+= 3){
        data.append(
            generatedTriangleShapedVertexBuffer[i - 2],
            generatedTriangleShapedVertexBuffer[i - 1],
            generatedTriangleShapedVertexBuffer[i]
        );
        data.appendUvs(
            generatedTriangleShapedUVBuffer[i - 2],
            generatedTriangleShapedUVBuffer[i - 1],
            generatedTriangleShapedUVBuffer[i]
        );
    }
}

void FTriangleClipResult::MakeUVBuffer(){
    generatedTriangleShapedUVBuffer.SetNum(generatedTriangleShapedVertexBuffer.Num());
    for (int i = 0; i < generatedTriangleShapedVertexBuffer.Num(); i++){
        const FVector &vertex = generatedTriangleShapedVertexBuffer[i];
        generatedTriangleShapedUVBuffer[i] = MakeUV(vertex);
    }
}

FVector2D FTriangleClipResult::MakeUV(const FVector &vertex){
    //scalar = distTarget / distAll
    FVector2D uv(
        vertex.X / fullBounds.X,
        vertex.Y / fullBounds.Y
    );
    uv += FVector2D(0.5, 0.5);
    return uv;
}
