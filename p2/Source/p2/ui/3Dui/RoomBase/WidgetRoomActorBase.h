#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "DebugPlugin/DebugHelper.h"
#include "AssetEnumCollection/assetEnums/EGameActorEnum.h"
#include "GameCore/PlayerControllerBase/PlayerControllerBase.h"
#include "p2/_world/EGameState.h"
#include "AssetPlugin/gamestart/assetManager.h"

#include "WidgetRoomActorBase.generated.h"

//instead of entering the room, the room will be moved to the player
UCLASS()
class P2_API AWidgetRoomActorBase : public AActorBase {
    GENERATED_BODY()

public:
    AWidgetRoomActorBase();

    //copies location of player and sets reference
    virtual bool Enter(AActor *actor);

    //player entered player will be reset!
    virtual void Leave();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    virtual void ClearReferencesOnEndPlay();

    void ResetPlayerLocation();

    FVector enteredLocation;
    AActor *playerEntered = nullptr;

    void LockPlayerMovement(bool flag);
    class APlayerControllerBase *GetPlayerEnteredControllerBase();

    void UpdateGameStateOnLeave(EGameState state);

private:




    
protected:
    /// ----- T Make Instance -----
    /// Shared for all 3d ui room Actors.
    template <typename T>
    static T *TMakeInstance(
        UWorld *world,
        EGameActorEnum typeActor,
        FVector Location
    )
    {
        static_assert(TIsDerivedFrom<T, AActor>::Value, "T must derive from AActor");

        if(!world){
            return nullptr;
        }

        UClass *SpawnClass = nullptr;
        if(assetManager *a = assetManager::instance()){
            SpawnClass = a->Find<EGameActorEnum, UClass>(typeActor);
        }
        if(!SpawnClass){
            return nullptr;
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        //FVector Location(-10000, -10000, -1000);
        T *spawned = world->SpawnActor<T>(
            SpawnClass,
            Location,
            FRotator::ZeroRotator,
            SpawnParams
        );

        if (!spawned)
        {
            DebugHelper::logMessage("AWidgetRoomActorBase::MakeInstance - Spawn actor failed");
            return nullptr;
        }

        //save instance ptr, only one needed.
        //instance = spawned;
        DebugHelper::logMessage("AWidgetRoomActorBase made instance");
        return spawned;
    }
};