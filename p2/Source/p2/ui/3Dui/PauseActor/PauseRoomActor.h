#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"

#include "p2/ui/3Dui/RoomBase/WidgetRoomActorBase.h"
#include "p2/ui/3Dui/PauseActor/EPauseWidgetEvent.h"

#include "PauseRoomActor.generated.h"

class APauseWidgetActor;
class UPauseWidget;

//instead of entering the room, the room will be moved to the player
UCLASS()
class P2_API APauseRoomActor : public AWidgetRoomActorBase {
    GENERATED_BODY()

private:
    static APauseRoomActor *instance;

public:
    APauseRoomActor();

    static void StaticEnter(AActor *player);
    static void StaticLeave();
    virtual bool Enter(AActor *player) override;
    virtual void Leave() override;

    

    void Notify(EPauseWidgetEvent event);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    static void CreateInstanceIfNeeded(AActor *actor);

    void NotifyExit();
    void NotifyOpenLoadoutRoom();
    void NotifyOpenGameStartRoom();

    void FindPauseMenuOnBeginPlay();
    void SetParentReferenceForPauseWidget();

    UPauseWidget *GetPauseWidget();

    virtual void ClearReferencesOnEndPlay() override;

    float distanceFromPlayerToBeKept = 150.0f;

    void SetToPlayerLocation();

private:
    APauseWidgetActor *pauseWidgetActor = nullptr;
};