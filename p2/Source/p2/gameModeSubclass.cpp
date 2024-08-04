// Fill out your copyright notice in the Description page of Project Settings.


#include "gameModeSubclass.h"
#include "EdgeCollector.h"
#include "Engine/World.h"

//constructor
AgameModeSubclass::AgameModeSubclass()
{
    // Set default pawn class to your Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Prefabs/player/playerPrefab")); // Adjust path if necessary
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // Set default pawn class
    //DefaultPawnClass = AplayerScript::StaticClass();

    //createPathFinder();
}

void AgameModeSubclass::BeginPlay()
{
    Super::BeginPlay();

    // Perform actions that depend on the world being ready
    createPathFinder();
}

void AgameModeSubclass::createPathFinder(){

    GLog->Flush();

    //get the world context
    UWorld* World = GetWorld();
    if (World)
    {
        EdgeCollector c = EdgeCollector();
        std::vector<FVector> edges = c.getAllEdges(World, 100.0f);

        showEdges(edges);

        FString string = FString::Printf(TEXT("collected %d"), edges.size());

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, string);
        }

        
    }
}


//shows the edges for debugging
void AgameModeSubclass::showEdges(std::vector<FVector> &edges){
    for (int i = 0; i < edges.size(); i++){
        FVector Start = edges.at(i);
        FVector End = edges.at(i) + FVector(0, 0, 1000);
        if (GetWorld())
        {
            DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 100.0f, 0, 1.0f);
        }
        
        if (GEngine)
        {
            FString string = FString::Printf(TEXT("%d"), Start.X);
            //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, string);
        }
    }
}
