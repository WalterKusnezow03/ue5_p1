#pragma once

#include "CoreMinimal.h"

#include "worldLevelBase.generated.h"

/**
 * acts as a task interface for plugins - processed in Sub Game Project!
 */



UCLASS()
class GAMECORE_API AworldLevelBase : public AActor{

    GENERATED_BODY()

protected:
    static AworldLevelBase *Instance;

public:
    AworldLevelBase();

    static void MakeInstanceBase(UWorld *world);

    static void addOutpostAt(FVector &location);

    static bool DebugSkelletonRecordMode(){
        return false;
    }

    static UWorld *GetWorldPointer();
    static void SetPlayerReference(AActor *player);
    static AActor* GetPlayerReference();

    template <typename T>
    static T* TGetPlayerReference(){
        if(AActor *raw = GetPlayerReference()){
            if(T* casted = Cast<T>(raw)){
                return casted;
            }
        }
        return nullptr;
    }

protected:
    virtual void OnPlayerReferenceSet(); //enter some game mode for example


    AActor *PlayerPointer = nullptr;

    TArray<FVector> outpostsToCreate;


    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    template <typename T>
    static T *Make(UWorld *world){
        static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T must be an AActor!");
        if(world != nullptr){

            UClass *toSpawn = T::StaticClass();
            if(toSpawn){
                
                FActorSpawnParameters SpawnParams;
                FVector Location;
                T *spawned = world->SpawnActor<T>(
                    T::StaticClass(), 
                    Location, 
                    FRotator::ZeroRotator, 
                    SpawnParams
                );
                return spawned;
            }
        }
        return nullptr;
    }
};