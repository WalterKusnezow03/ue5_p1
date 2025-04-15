
#include "StaticMeshBoundsextractor.h"
#include "p2/DebugHelper.h"

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
    //rotation.Pitch += 90.0f; //sodass Z = 0.0f wird, xy pane only

    FMatrix RotationMatrix = FRotationMatrix(rotation);
    FMatrix InverseMatrix = RotationMatrix.Inverse();

    TArray<FVector> bufferOnXYPane;
    TArray<FVector> rawBuffer;
    rawBuffer.SetNum(verteciesIn.Num());
    bufferOnXYPane.SetNum(verteciesIn.Num());
    for (int i = 0; i < verteciesIn.Num(); i++){
        FVector copy(verteciesIn[i]);
        rawBuffer[i] = copy;

        copy -= paneLocalLocation; //translate to relative 0,0,0
        bufferOnXYPane[i] = InverseMatrix.TransformPosition(copy);

        //actung: ganzer buffer muss noch in bezug auf pane geschoben werden, abstand pane vom lokalen
        //ursprung!
        //translation wird auf zurückgesetzt



        DebugHelper::logMessage("extracted rotated ", bufferOnXYPane[i]);
    }

    TArray<int32> inBoundsVertecies;
    FindInBounds(a, b, bufferOnXYPane, inBoundsVertecies);

    //found vertecies in bounds: copy corrosponding triangles
    copy(rawBuffer, inBoundsVertecies, trianglesIn, actorLocation);



    //debug draw normal
    normal = InverseMatrix.TransformPosition(normal);
    DebugHelper::logMessage("extracted normal rotated ", normal); //looks up: OK!
    




}

void StaticMeshBoundsextractor::FindInBounds(
    FVector2D &a,
    FVector2D &b,
    TArray<FVector> &verteciesIn,
    TArray<int32> &verteciesOut
){
    double nearestVertexZ = -9999999999.0;
    for (int i = 0; i < verteciesIn.Num(); i++){
        FVector &vertex = verteciesIn[i];
        nearestVertexZ = std::max(nearestVertexZ, vertex.Z);
    }

    float aval = nearestVertexZ;
    DebugHelper::logMessage(FString::Printf(TEXT("extracted nearestValue %.2f"), aval));

    for (int i = 0; i < verteciesIn.Num(); i++)
    {
        FVector &vertex = verteciesIn[i];
        if(isInBounds(a,b,vertex, nearestVertexZ)){
            verteciesOut.Add(i);
        }
    }
}

bool StaticMeshBoundsextractor::isInBounds(
    FVector2D &a,
    FVector2D &b,
    FVector &vertex,
    double nearestVertexZ
){
    return a.X <= vertex.X &&
           a.Y <= vertex.Y &&
           b.X >= vertex.X &&
           b.Y >= vertex.Y &&
           vertex.Z >= nearestVertexZ -50.0f; //zu prüfen, das der abstand quasi reicht
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
                actorLocation + rawBuffer[index] * 1.1f,
                actorLocation,
                FColor::Orange,
                100.0f
            );
        }
    }
}
