#include "BufferClipper.h"


#include "customUiPlugin/slate/MeshData2D/BaseMeshData/SlateMeshData.h"
SlateMeshDataPolygon BufferClipper::Test(){
    TArray<FVector2D> buffer = {
        FVector2D(0, 0),
        FVector2D(10, 0),
        FVector2D(20, 5),
        FVector2D(20, 20),
        FVector2D(10, 0)
    };
    TArray<int32> triangles = {
        0,1,2,
        0,2,3,
        0,3,4
    };

    FVector2D a(5, 5);
    FVector2D b(50, 50);

    TArray<FVector2D> outBuffer; // must be empty
    TArray<int32> outTriangles;

    SlateMeshDataPolygon outpolygon;
    SlateMeshData &data = outpolygon.MeshDataRef();
    ClipBuffer(
        buffer,
        triangles,
        a,
        b,
        outBuffer,   // must be empty
        outTriangles // must be empty
    );

    data.AppendTriangleShapedBuffer(outBuffer);

    return outpolygon;
}

//cannot clamp when shape smaller on more than 1 side ?
void BufferClipper::ClipBuffer(
    const TArray<FVector2D> &buffer,
    const TArray<int32> &triangles,
    FVector2D &topLeft,
    FVector2D &bottomRight,
    TArray<FVector2D> &outBuffer, //must be empty
    TArray<int32> &outTriangles //must be empty
){
    TArray<FClipEdge> edges = MakeEdgesFromQuad(topLeft, bottomRight);

    //clip triangles against edges and add triangle to buffer.
    TArray<FVector2D> outBufferTriangleShaped;
    for (int i = 2; i < triangles.Num(); i += 3){
        FVector2D v0 = buffer[triangles[i - 2]]; //if oob access thats not my fault.
        FVector2D v1 = buffer[triangles[i - 1]];
        FVector2D v2 = buffer[triangles[i]];

        TArray<FVector2D> clipped;
        for (int edge = 0; edge < edges.Num(); edge++)
        {
            int outside = 0;
            bool v0Inside = true;
            bool v1Inside = true;
            bool v2Inside = true;

            FClipEdge &currentEdge = edges[edge];
            if(currentEdge.RightOffWorld(v0)){
                outside++;
                v0Inside = false;
            }
            if(currentEdge.RightOffWorld(v1)){
                outside++;
                v1Inside = false;
            }
            if(currentEdge.RightOffWorld(v2)){
                outside++;
                v2Inside = false;
            }

            if(outside > 0){
                if(outside == 2){
                    //einfach alle clippen?
                    if(v0Inside){
                        ClipOneInside(v0, v1, v2, currentEdge, clipped);
                    }
                    if(v1Inside){
                        ClipOneInside(v1, v2, v0, currentEdge, clipped);
                    }

                    if(v2Inside){
                        ClipOneInside(v2, v0, v1, currentEdge, clipped);
                    }
                }
                if(outside == 1){
                    if(v0Inside && v2Inside){
                        ClipTwoInside(v0, v1, v2, currentEdge, clipped);
                    }
                    if(v0Inside && v1Inside){
                        ClipTwoInside(v0, v2, v1, currentEdge, clipped);
                    }
                    if(v1Inside && v2Inside){
                        ClipTwoInside(v1, v0, v2, currentEdge, clipped);
                    }
                }
                break;
            }else{
                clipped.Add(v0);
                clipped.Add(v1);
                clipped.Add(v2);
            }
            outBufferTriangleShaped.Append(clipped);
        }
    }

    outBuffer = outBufferTriangleShaped;
    GenerateTriangleBuffer(outBufferTriangleShaped, outTriangles);
}

void BufferClipper::ClipOneInside(
    FVector2D &v0_inside, 
    FVector2D &v1, 
    FVector2D &v2,
    FClipEdge &currentEdge,
    TArray<FVector2D> &appendBuffer
){
   
    appendBuffer.Add(v0_inside);
    currentEdge.ClipEndWorld(v0_inside, v1);
    appendBuffer.Add(v1);
    currentEdge.ClipEndWorld(v0_inside, v2);
    appendBuffer.Add(v2);
}


void BufferClipper::ClipTwoInside(
    FVector2D &v0_inside, 
    FVector2D &v1, 
    FVector2D &v2_inside,
    FClipEdge &currentEdge,
    TArray<FVector2D> &appendBuffer
){
   
    FVector2D v1copy = v1;

    //triangle A
    appendBuffer.Add(v0_inside);
    currentEdge.ClipEndWorld(v0_inside, v1copy);
    appendBuffer.Add(v1copy);
    appendBuffer.Add(v2_inside);

    //triangle B
    appendBuffer.Add(v2_inside);
    appendBuffer.Add(v1copy);
    currentEdge.ClipEndWorld(v2_inside, v1);
    appendBuffer.Add(v1);
}


void BufferClipper::GenerateTriangleBuffer(
    TArray<FVector2D> &vertexBufferTriangleShaped, 
    TArray<int32> &outTriangles
){
    outTriangles.SetNum(vertexBufferTriangleShaped.Num());
    for (int i = 0; i < vertexBufferTriangleShaped.Num(); i ++){
        outTriangles[i] = i;
    }
}

TArray<FClipEdge> BufferClipper::MakeEdgesFromQuad(
    const FVector2D &topLeft,
    const FVector2D &bottomRight
){
    /*
    logic
    1-->2
    |   |
    0<--3 

    screen 
    0<--3
    |   |
    1-->2
    */
    FVector2D v0 = topLeft;
    FVector2D v1 = FVector2D(topLeft.X, bottomRight.Y);
    FVector2D v2 = bottomRight;
    FVector2D v3 = FVector2D(bottomRight.X, topLeft.Y);

    TArray<FClipEdge> edges;
    edges.Add(MakeEdge(v0, v1));
    edges.Add(MakeEdge(v1, v2));
    edges.Add(MakeEdge(v2, v3));
    edges.Add(MakeEdge(v3, v0));
    return edges;
}

FClipEdge BufferClipper::MakeEdge(
    const FVector2D &a,
    const FVector2D &b
){
    FVector2D dir = b - a;
    return FClipEdge(a, dir);
}