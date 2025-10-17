
#include "StaticMeshBoundsextractor.h"
#include "DebugPlugin/DebugHelper.h"

/**
 * 
 * 
 * ----- bonds copy ------
 * 
 * 
 */

StaticMeshBoundsextractor::StaticMeshBoundsextractor(){

}
StaticMeshBoundsextractor::~StaticMeshBoundsextractor(){

}


void StaticMeshBoundsextractor::CopyInBounds(
    FVector2D &a,//ebene muss in koordinaten des eigenen meshes liegen (x,y ebene ideal)
    FVector2D &b,
    FVector &normal, //normale der ebene / flat pane
    TArray<FVector3f> &verteciesIn,
    TArray<int32> &trianglesIn,
    FVector paneLocalLocation,
    FVector actorLocation,
    UWorld *worldIn
){
    world = worldIn;

    //by normal depth richtung z.b. -50cm max

    //dafür vertecies projezieren auf xy ebene
    normal = normal.GetSafeNormal();
    FRotator rotation = normal.Rotation();
    
    //DBUEG
    rotation.Pitch += 90.0f; //sodass Z = 1.0f wird, xy pane only
    //Z schaut nach -1.0f

    FMatrix RotationMatrix = FRotationMatrix(rotation);
    FMatrix InverseMatrix = RotationMatrix.Inverse();

    TArray<FVector> bufferOnXYPane;
    TArray<FVector> rawBuffer;
    rawBuffer.SetNum(verteciesIn.Num());
    bufferOnXYPane.SetNum(verteciesIn.Num());
    for (int i = 0; i < verteciesIn.Num(); i++){
        FVector copy(verteciesIn[i]);
        rawBuffer[i] = copy;

        //actung: ganzer buffer muss noch in bezug auf pane geschoben werden, abstand pane vom lokalen
        //ursprung!
        //translation wird auf zurückgesetzt

        copy -= paneLocalLocation; //translate to relative 0,0,0
        bufferOnXYPane[i] = InverseMatrix.TransformPosition(copy);



        DebugHelper::logMessage("extracted rotated ", bufferOnXYPane[i]);
    }


    //debug draw normal
    normal = InverseMatrix.TransformPosition(normal);
    DebugHelper::logMessage("extracted normal rotated ", normal); //looks up: OK!

    TArray<int32> inBoundsVertecies;
    FindInBounds(a, b, bufferOnXYPane, inBoundsVertecies, normal);

    //found vertecies in bounds: copy corrosponding triangles
    copy(rawBuffer, inBoundsVertecies, trianglesIn, actorLocation);



    for (int i = 1; i < bufferOnXYPane.Num(); i++){
        DebugHelper::showLineBetween(
            world,
            bufferOnXYPane[i] + actorLocation + paneLocalLocation,
            bufferOnXYPane[i - 1] + actorLocation + paneLocalLocation,
            FColor::Green,
            100.0f
        );
    }
}

void StaticMeshBoundsextractor::FindInBounds(
    FVector2D &a,
    FVector2D &b,
    TArray<FVector> &verteciesIn,
    TArray<int32> &verteciesOut,
    FVector rotatedNormal
){

    //ERST XY BOUNDS


    //DANN DEPTH CHECK
    TArray<int32> inBoundsXY;
    for (int i = 0; i < verteciesIn.Num(); i++){
        FVector &vertex = verteciesIn[i];
        if(isInBounds(a,b,vertex)){
            inBoundsXY.Add(i);
        }
    }


    double nearestVertexZ = rotatedNormal.Z > 0.0f ? -9999999999.0 : 9999999999.0;
    for (int i = 0; i < inBoundsXY.Num(); i++){
        FVector &vertex = verteciesIn[inBoundsXY[i]];

        if(rotatedNormal.Z > 0.0f){
            nearestVertexZ = std::max(nearestVertexZ, vertex.Z); //weil z nach -1 zeigt.
        }else{
            nearestVertexZ = std::min(nearestVertexZ, vertex.Z); //weil z nach -1 zeigt.
        }
    }


    //DEPTH CHECK
    for (int i = 0; i < inBoundsXY.Num(); i++){
        int index = inBoundsXY[i];
        FVector &vertex = verteciesIn[index];
        if(vertex.Z >= nearestVertexZ -50.0f && vertex.Z < nearestVertexZ + 50.0f){
            verteciesOut.Add(index);
        }
    }

}

bool StaticMeshBoundsextractor::isInBounds(
    FVector2D &a,
    FVector2D &b,
    FVector &vertex
){
    return a.X <= vertex.X &&
           a.Y <= vertex.Y &&
           b.X >= vertex.X &&
           b.Y >= vertex.Y;
}

void StaticMeshBoundsextractor::copy(
    TArray<FVector> &rawBuffer,
    TArray<int32> &inBoundsVertecies,
    TArray<int32> &trianglesIn,
    FVector actorLocation
){

    DebugHelper::logMessage(
        FString::Printf(TEXT(
            "extracted vertecies: %d, of %d"),
            inBoundsVertecies.Num(),
            rawBuffer.Num()
        )
    );

    //draw
    FVector locationDraw(-100, 0, 0);
    actorLocation += locationDraw;

    for (int i = 0; i < inBoundsVertecies.Num(); i++){
        int32 index = inBoundsVertecies[i];
        if(index >= 0 && index < rawBuffer.Num()){
            DebugHelper::showLineBetween(
                world,
                actorLocation + rawBuffer[index] * 1.1f + FVector(0, 300,20),
                actorLocation,
                FColor::Orange,
                100.0f
            );
        }
    }
}
