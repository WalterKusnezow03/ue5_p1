// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshResources.h" 
#include "GameFramework/Actor.h"
#include "p2/Damageinterface.h"
#include "Engine/StaticMeshActor.h"
#include "MeshDescription.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"
#include <cmath>
#include "EdgeCollector.h"

EdgeCollector::EdgeCollector()
{
    readEdges = new std::vector<FVector>();
    edgeDataEdges = new std::vector<edgeData>();
    worldIn = nullptr;
}

EdgeCollector::~EdgeCollector()
{
    if(readEdges != nullptr){
        delete (readEdges);
    }
    if(edgeDataEdges != nullptr){
        delete (edgeDataEdges);
    }
}


/// @brief returns the already read edges
/// @return edge vector 
std::vector<FVector>& EdgeCollector::getReadEdges(){
    return *readEdges;
}





//Function to process static meshes in the world
//returns a reference to the edges, copy needed because object will destroy them on destruct!

/// @brief will get all edges from the given world.
/// @param World 
/// @param minHeight 
/// @return std::vector<FVector>& a list of nodes for your pathfinding algorythm
std::vector<FVector>& EdgeCollector::getAllEdges(UWorld* World, float minHeight)
{
    worldIn = World;
    edgeMinHeight = minHeight;
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid World context."));
        return *readEdges;
    }

    // Iterate over all actors in the world
    TArray<AActor*> AllActors;
    //alle actors in das allactors array
    UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), AllActors);

    for (int32 i = 0; i < AllActors.Num(); ++i)
    {
        AActor* Actor = AllActors[i];
        if (Actor)
        {
            //exclude enteties
            IDamageinterface *damageInterface = Cast<IDamageinterface>(Actor);
            if(damageInterface == nullptr){
                // Iterate over all components of the actor
                TArray<UActorComponent*> array;
                Actor->GetComponents(array);

                //childs
                for (UActorComponent* component : array)
                {
                    //if component is a mesh component
                    if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(component))
                    {
                        getEdgesFromSingleMeshComponent(MeshComponent, *edgeDataEdges);
                    }
                }
            }
        }

    }

    
    

    
    //copy edges bottom
    for (int i = 0; i < edgeDataEdges->size(); i++){
        //FVector t = edgeDataEdges->at(i).bottom + (edgeDataEdges->at(i).top - edgeDataEdges->at(i).bottom) * 0.1f;
        readEdges->push_back(edgeDataEdges->at(i).bottom);
        //readEdges->push_back(t);
    }

    return *readEdges; //dereference and return
}


//collect adges for an single actor passed per pointer
//data will get append to array
void EdgeCollector::getEdgesForActor(AActor* actor, std::vector<FVector> &vector){
    if(actor){
        // Iterate over all components of the actor
        TArray<UActorComponent*> array;
        actor->GetComponents(array);

        std::vector<edgeData> vectorEdges;

        //childs
        for (UActorComponent* component : array)
        {
            //if component is a mesh component
            if (UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(component))
            {
                getEdgesFromSingleMeshComponent(MeshComponent, vectorEdges); //todo: übergabe des arrays?
            }
        }

        

        
        //copy edges bottom
        for (int i = 0; i < vectorEdges.size(); i++){
            vector.push_back(vectorEdges.at(i).bottom);
        }
    }
}



// reads static mesh from static mesh component
void EdgeCollector::getEdgesFromSingleMeshComponent(
    UStaticMeshComponent* MeshComponent,
    std::vector<edgeData> &vector
)
{
    if (MeshComponent && MeshComponent->GetStaticMesh())
    {
        //needs the transform to revert the world space
        const FTransform LocalToWorldTransform = MeshComponent->GetComponentTransform();

        getEdgesFromSingleMesh(
            MeshComponent->GetStaticMesh(), 
            MeshComponent->GetComponentLocation(),
            LocalToWorldTransform,
            vector
        );
    }

    return;
}


// Function to get all vertical edges of a mesh as copy
void EdgeCollector::getEdgesFromSingleMesh(
    UStaticMesh* StaticMesh, 
    FVector debugPos, 
    FTransform LocalToWorldTransform,
    std::vector<edgeData> &vector
){
    if (!StaticMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("StaticMesh is null."));
        return;
    }

    // Ensure we have the render data for the mesh
    const FStaticMeshRenderData* RenderData = StaticMesh->GetRenderData();
    if (!RenderData) 
    {
        UE_LOG(LogTemp, Warning, TEXT("RenderData is null."));
        return;
    }

    //showPos(worldIn, debugPos);

    std::vector<edgeData> currentEdges = std::vector<edgeData>();

    // Iterate through all LODs (Levels of Detail)
    for (int32 LODIndex = 0; LODIndex < RenderData->LODResources.Num(); LODIndex++)
    {
        const FStaticMeshLODResources& LODResources = RenderData->LODResources[LODIndex];

        // Iterate through all the vertex buffers
        const FPositionVertexBuffer& PositionVertexBuffer = LODResources.VertexBuffers.PositionVertexBuffer;
        const FStaticMeshVertexBuffer& StaticMeshVertexBuffer = LODResources.VertexBuffers.StaticMeshVertexBuffer;

        /*
        remember:
        1--2
        |  |
        0<-3
        vertecy anordnung
        */

        // Iterate through each vertex
        for (uint32 VertexIndex = 1; VertexIndex < PositionVertexBuffer.GetNumVertices(); VertexIndex++)
        {
            
            //matrix transformation durchführen um vector korrekt zu lesen
            FVector A = LocalToWorldTransform.TransformPosition(
                FVector(PositionVertexBuffer.VertexPosition(VertexIndex - 1))
            );

            FVector B = LocalToWorldTransform.TransformPosition(
                FVector(PositionVertexBuffer.VertexPosition(VertexIndex))
            );

            
            //Wenn vertikale kante: interessiert daran
            if(isVertical(A, B)){

                bool extended = false;
                // check ob vorherige edge vertikal erweitert
                if(currentEdges.size() > 0){

                    extended = false;

                    edgeData &prev = currentEdges.back(); //reference the back
                    FVector lower = A.Z < B.Z ? A : B;
                    FVector higher = A.Z > B.Z ? A : B;
                    if (FVector::Dist(prev.top, lower) <= 10) // 10cm
                    { 
                        prev.top = higher;
                        extended = true;
                    }
                }

                //if not extended vertically, just add
                if(!extended){
                    if(A.Z < B.Z){
                        currentEdges.push_back(edgeData(A, B));
                    }else{
                        currentEdges.push_back(edgeData(B, A)); //hier ein punkt weil lokal ref
                    }
                }



                /*
                //default adding
                //nach höhe einsortieren
                if(A.Z < B.Z){
                    currentEdges.push_back(edgeData(A, B));
                }else{
                    currentEdges.push_back(edgeData(B, A)); //hier ein punkt weil lokal ref
                }*/

                
            }
        }
    }

    //pass by reference
    ComputeConvexHull(currentEdges);


    //caluclate raycast hits and apply to all edges aligning them properly
    collectRaycasts(currentEdges, worldIn);



    // insert(start orig, start appended, end appended);
    //edgeDataEdges->insert(edgeDataEdges->end(), currentEdges.begin(), currentEdges.end());
    vector.insert(vector.end(), currentEdges.begin(), currentEdges.end());

}


//checks if an edge is vertical
bool EdgeCollector::isVertical(FVector A, FVector B){

    FVector connect = B - A;
    if(A.Z > B.Z){
        connect = A - B;
    }

    FVector connectNormal = connect.GetSafeNormal(); // AB = B - A

    float skalarProduktUp = std::abs(connectNormal.Z);

    return skalarProduktUp > 0.7f; //edge of interest
}




//will compute the convex hull and write into the passed array
void EdgeCollector::ComputeConvexHull(std::vector<edgeData> &points) { //passed by reference also kann ich reinkritzeln
    std::vector<edgeData> convexHull; //value, will be removed from stack

    if (points.size() < 3) {
        return;
    }

    // Sort points to calculate the lower hull
    std::sort(points.begin(), points.end(), [](const edgeData& a, const edgeData& b) {
        if (a.top.X == b.top.X) {
            return a.top.Y < b.top.Y;
        }
        return a.top.X < b.top.X;
    });

    // Calculate the lower hull
    for (const edgeData& point : points) {
        while (convexHull.size() >= 2 && !IsClockwise(convexHull[convexHull.size() - 2], convexHull.back(), point)) {
            convexHull.pop_back();
        }
        convexHull.push_back(point);
    }

    // Calculate the upper hull
    int lowerHullCount = convexHull.size();
    for (int i = points.size() - 2; i >= 0; --i) {
        edgeData point = points[i];
        while (
            convexHull.size() > lowerHullCount && 
            !IsClockwise(convexHull[convexHull.size() - 2], convexHull.back(), point)
        ) {
            convexHull.pop_back();
        }
        convexHull.push_back(point);
    }

    // Remove the last point which might be duplicated
    if (!convexHull.empty()) {
        convexHull.pop_back();
    }


    //copy data to passed array
    int cSize = convexHull.size();

    //Overwrite elements
    for (int i = 0; i < cSize; ++i) {
        if (i < points.size()) {
            points[i] = convexHull[i];
        }
    }

    //Remove the excess elements
    if (cSize < points.size()) {
        points.erase(points.begin() + cSize, points.end());
    }

    return;
    //convexHull;
}


bool EdgeCollector::IsClockwise(const edgeData& a, const edgeData& b, const edgeData& c) {
    return (b.top.X - a.top.X) * (c.top.Y - a.top.Y) - (b.top.Y - a.top.Y) * (c.top.Y - a.top.Y) < 0;
}





//shows the edges for debugging
void EdgeCollector::showEdges(std::vector<edgeData> &edges, UWorld *world){
    for (int i = 0; i < edges.size(); i++){
        FVector Start = edges.at(i).top;
        FVector End = edges.at(i).bottom;
        DrawDebugLine(world, Start, End, FColor::Green, false, 10.0f, 0, 5.0f);
    }
}


void EdgeCollector::showPos(UWorld *world, FVector e){
    FVector End = e + FVector(0, 0, 50000);
    DrawDebugLine(world, e, End, FColor::Green, true, 10.0f, 100, 5.0f);
}

//draws a line from point a to b
void EdgeCollector::showLine(FVector e, FVector g){
    if(worldIn != nullptr){
        DrawDebugLine(worldIn, e, g, FColor::Yellow, true, 10.0f, 100, 5.0f);
    }
    
}







//inner class constructor
EdgeCollector::edgeData::edgeData(FVector bottomIn, FVector topIn){
    top = topIn;
    bottom = bottomIn;
}

//detsructor of inner class
EdgeCollector::edgeData::~edgeData(){

}



/// @brief applies the raycast bottom position to the edges
/// @param edges 
/// @param world 
void EdgeCollector::collectRaycasts(std::vector<edgeData> &edges, UWorld *world){

    //calculate center pos for offset
    FVector centerTop;
    FVector centerBottom;
    FVector center;
    for (int i = 0; i < edges.size(); i++)
    {
        centerTop += edges.at(i).top;
        centerBottom += edges.at(i).bottom;
        center += edges.at(i).top + edges.at(i).bottom;
    }
    centerTop /= (edges.size());
    centerBottom /= (edges.size());
    center /= (edges.size() * 2);

    //apply offset smallest //PUSHOUT EDGE
    for (int i = 0; i < edges.size(); i++) {
        FVector top = edges.at(i).top;
        FVector bottom = edges.at(i).bottom;

        //FVector dir = (top - center).GetSafeNormal();
        FVector dir = (top - center).GetSafeNormal();
        dir.Z = 0;
        edges.at(i).top += dir * 150; //PUSHOUT 150! 
        edges.at(i).bottom += dir * 150;
        
    }
    
    // create raycasts
    for (int i = 0; i < edges.size(); i++)
    {
        collectRaycast(edges.at(i), world);
    }
}

void EdgeCollector::collectRaycast(edgeData &edge, UWorld *world){
    if(world){
        FVector Start = edge.top;
        Start.Z += 1000;
        FVector End = edge.bottom;
        End.Z -= 1000;

        FHitResult HitResult;
		FCollisionQueryParams Params;
		//Params.AddIgnoredActor(this); // Ignore the character itself

		bool bHit = world->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);


		// If the raycast hit something, log the hit actor's name
		if (bHit)
		{
            //needed here to check if the distance between hit and start is less than 10% of distance
            //to prevent false edges
            float completeDistance = FVector::Dist(Start, edge.top);
            float hitDistanceFromTop = FVector::Dist(Start, HitResult.ImpactPoint);
            if(completeDistance * 0.2f < hitDistanceFromTop){
                //min 20% distance to remove false hits

                FVector hitPos = HitResult.ImpactPoint;
                hitPos.Z += 70;
                edge.bottom = hitPos;
            }


            
        }
    }
    //return nullptr;
}